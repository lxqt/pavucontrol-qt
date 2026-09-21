/***
  This file is part of pavucontrol.

  Copyright 2006-2008 Lennart Poettering
  Copyright 2008 Sjoerd Simons <sjoerd@luon.net>

  pavucontrol is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  pavucontrol is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with pavucontrol. If not, see <https://www.gnu.org/licenses/>.
***/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define PACKAGE_VERSION "0.1"

#include <csignal>

#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>
#include <pulse/ext-stream-restore.h>
#include <pulse/ext-device-manager.h>

// #include <canberra-gtk.h>

#include "pavucontrol.h"
#include "streamwidget.h"
#include "sinkinputwidget.h"
#include "sourceoutputwidget.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include <QLocale>
#include <QLibraryInfo>
#include <QTranslator>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

static pa_context* context = nullptr;
static pa_mainloop_api* api = nullptr;
static int n_outstanding = 0;
static int default_tab = 0;
static bool retry = false;
static int reconnect_timeout = 1;

void show_error(const char *txt) {
    char buf[256];

    snprintf(buf, sizeof(buf), "%s: %s", txt, pa_strerror(pa_context_errno(context)));

    QMessageBox::critical(nullptr, QObject::tr("Error"), QString::fromUtf8(buf));
    qApp->quit();
}

static void dec_outstanding(MainWindow *w) {
    if (n_outstanding <= 0)
        return;

    if (--n_outstanding <= 0) {
        // w->get_window()->set_cursor();
        w->setConnectionState(true);
    }
}

#if HAVE_PULSE_MESSAGING_API

/*
 * Bluetooth codec selection (e.g. switching a headset between SBC/AAC/LDAC)
 * is not exposed through the normal pa_context_get_card_info()/pa_card_info
 * API. Instead, PulseAudio's module-bluez5-device registers a generic
 * "message handler" per Bluetooth card, reachable through the object
 * messaging API added in PulseAudio 15.0 (pa_context_send_message_to_object(),
 * see pulse/introspect.h and https://cgit.freedesktop.org/pulseaudio/pulseaudio/tree/doc/messaging_api.txt).
 *
 * The handler lives at path "/card/<pulse card name>/bluez" and understands
 * three messages:
 *   - "list-codecs" -> returns a JSON array of {"name": ..., "description": ...}
 *                       describing every codec this device could use
 *   - "get-codec"   -> returns a bare JSON string with the currently active
 *                       codec's name (or JSON null if nothing is active yet)
 *   - "switch-codec"-> takes a bare JSON string (the codec name) as its
 *                       message_parameters and switches to it
 *
 * Because most cards are not Bluetooth devices at all, and plenty of servers
 * (older PulseAudio, or PipeWire's pulse-compat layer, which may or may not
 * implement this specific handler) won't have this handler registered, every
 * failure in this chain is treated as "this card doesn't support codec
 * switching" rather than as an error: it must NEVER call show_error(), since
 * show_error() calls qApp->quit() — that's correct for genuinely fatal,
 * connection-level failures elsewhere in this file, but here it would mean
 * a single unremarkable, totally expected non-Bluetooth card would quit the
 * whole application.
 */

/* Path of the per-card Bluetooth codec-switching message handler, if the
 * running PulseAudio/module-bluez5-device registered one for this card. */
QByteArray bluezMessageHandlerPath(const QByteArray &cardName) {
    return "/card/" + cardName + "/bluez";
}

/* Carries the MainWindow + PulseAudio card name across one async message
 * round-trip. Every send below allocates one of these as the pa_operation's
 * userdata and the matching callback below is responsible for deleting it
 * exactly once, on every path (success, failure, or "not applicable"). */
struct CardCodecQuery {
    MainWindow *window;
    QByteArray cardName;
};

/* Sends a codec-related message to a registered PulseAudio message handler.
 * Best-effort only: most cards (and servers that don't implement the
 * messaging API at all) simply won't have a handler at the given path, so
 * failures here are routine and must never be treated as fatal.
 *
 * If pa_context_send_message_to_object() itself fails synchronously (no
 * pa_operation is created), `cb` will never fire, so the CardCodecQuery is
 * freed here instead to avoid leaking it. */
static void sendCodecMessage(const QByteArray &path, const char *message, MainWindow *w, const QByteArray &cardName, pa_context_string_cb_t cb) {
    pa_operation *o;
    auto *q = new CardCodecQuery{w, cardName};

    if ((o = pa_context_send_message_to_object(context, path.constData(), message, nullptr, cb, q)))
        pa_operation_unref(o);
    else
        delete q;
}

/* Parses a "list-codecs" or "list-handlers" style response: a JSON array of
 * {"name": "...", "description": "..."} objects. Used both for the codec
 * list itself and (in context_message_handlers_cb below) to search the
 * handler list for our target path — same shape, different meaning of the
 * name/description pair. Malformed or unexpected JSON just yields an empty
 * result rather than an error. */
static std::vector<std::pair<QByteArray, QByteArray>> parseCodecArray(const QJsonDocument &doc) {
    std::vector<std::pair<QByteArray, QByteArray>> codecs;

    if (!doc.isArray())
        return codecs;

    for (const QJsonValue &v : doc.array()) {
        if (!v.isObject())
            continue;
        QJsonObject o = v.toObject();
        codecs.emplace_back(o.value(QStringLiteral("name")).toString().toUtf8(),
                             o.value(QStringLiteral("description")).toString().toUtf8());
    }

    return codecs;
}

/* Response callback for "list-codecs": hands the parsed codec list to the
 * CardWidget matching q->cardName so it can populate the codec combo box. */
static void context_bluetooth_codec_list_cb(pa_context *, int success, char *response, void *userdata) {
    auto *q = static_cast<CardCodecQuery*>(userdata);

    if (success && response)
        q->window->updateCardCodecs(q->cardName, parseCodecArray(QJsonDocument::fromJson(response)));

    delete q;
}

/* Response callback for "get-codec": the response is a *bare* JSON string
 * (e.g. `"ldac"`) or bare `null`, not wrapped in an object or array.
 * QJsonDocument::fromJson() only accepts an object or array at the top
 * level (per Qt's JSON implementation), so a bare scalar would otherwise
 * fail to parse even though it's valid JSON by the RFC. Wrapping it in "["
 * ... "]" turns it into a single-element array QJsonDocument is happy to
 * parse, and we then just read that one element back out. */
static void context_bluetooth_active_codec_cb(pa_context *, int success, char *response, void *userdata) {
    auto *q = static_cast<CardCodecQuery*>(userdata);

    if (success && response) {
        QJsonDocument doc = QJsonDocument::fromJson("[" + QByteArray(response) + "]");
        if (doc.isArray() && doc.array().size() == 1) {
            QJsonValue v = doc.array().at(0);
            if (v.isString())
                q->window->setActiveCodec(q->cardName, v.toString().toUtf8());
        }
    }

    delete q;
}

/* Response callback for "list-handlers" sent to the global "/core" object.
 * This is the entry point of the whole chain (kicked off from card_cb below
 * for every card): it lists every message handler currently registered
 * anywhere on the server, and we check whether our card's bluez handler
 * path is among them. Only if it is do we know this card actually supports
 * codec switching, and only then do we go on to ask for the codec list and
 * the currently active codec. */
static void context_message_handlers_cb(pa_context *, int success, char *response, void *userdata) {
    auto *q = static_cast<CardCodecQuery*>(userdata);

    if (success && response) {
        QByteArray targetPath = bluezMessageHandlerPath(q->cardName);
        bool found = false;

        for (const QJsonValue &v : QJsonDocument::fromJson(response).array()) {
            if (v.isObject() && v.toObject().value(QStringLiteral("name")).toString().toUtf8() == targetPath) {
                found = true;
                break;
            }
        }

        if (found) {
            sendCodecMessage(targetPath, "get-codec", q->window, q->cardName, context_bluetooth_active_codec_cb);
            sendCodecMessage(targetPath, "list-codecs", q->window, q->cardName, context_bluetooth_codec_list_cb);
        }
    }

    delete q;
}

#endif

void card_cb(pa_context *, const pa_card_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        show_error(QObject::tr("Card callback failure").toUtf8().constData());
        return;
    }

    if (eol > 0) {
        dec_outstanding(w);
        return;
    }

    w->updateCard(*i);

#if HAVE_PULSE_MESSAGING_API
    sendCodecMessage("/core", "list-handlers", w, QByteArray(i->name), context_message_handlers_cb);
#endif
}

#if HAVE_EXT_DEVICE_RESTORE_API
static void ext_device_restore_subscribe_cb(pa_context *c, pa_device_type_t type, uint32_t idx, void *userdata);
#endif

void sink_cb(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        show_error(QObject::tr("Sink callback failure").toUtf8().constData());
        return;
    }

    if (eol > 0) {
        dec_outstanding(w);
        return;
    }
#if HAVE_EXT_DEVICE_RESTORE_API
    if (w->updateSink(*i))
        ext_device_restore_subscribe_cb(c, PA_DEVICE_TYPE_SINK, i->index, w);
#else
    w->updateSink(*i);
#endif
}

void source_cb(pa_context *, const pa_source_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        show_error(QObject::tr("Source callback failure").toUtf8().constData());
        return;
    }

    if (eol > 0) {
        dec_outstanding(w);
        return;
    }

    w->updateSource(*i);
}

void sink_input_cb(pa_context *, const pa_sink_input_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        show_error(QObject::tr("Sink input callback failure").toUtf8().constData());
        return;
    }

    if (eol > 0) {
        dec_outstanding(w);
        return;
    }

    w->updateSinkInput(*i);
}

void source_output_cb(pa_context *, const pa_source_output_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        show_error(QObject::tr("Source output callback failure").toUtf8().constData());
        return;
    }

    if (eol > 0)  {

        if (n_outstanding > 0) {
            /* At this point all notebook pages have been populated, so
             * let's open one that isn't empty */
            if (default_tab != -1) {
                if (default_tab < 1 || default_tab > w->notebook->count()) {
                    if (!w->sinkInputWidgets.empty())
                        w->notebook->setCurrentIndex(0);
                    else if (!w->sourceOutputWidgets.empty())
                        w->notebook->setCurrentIndex(1);
                    else if (!w->sourceWidgets.empty() && w->sinkWidgets.empty())
                        w->notebook->setCurrentIndex(3);
                    else
                        w->notebook->setCurrentIndex(2);
                } else {
                    w->notebook->setCurrentIndex(default_tab - 1);
                }
                default_tab = -1;
            }
        }

        dec_outstanding(w);
        return;
    }

    w->updateSourceOutput(*i);
}

void client_cb(pa_context *, const pa_client_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        show_error(QObject::tr("Client callback failure").toUtf8().constData());
        return;
    }

    if (eol > 0) {
        dec_outstanding(w);
        return;
    }

    w->updateClient(*i);
}

void server_info_cb(pa_context *, const pa_server_info *i, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (!i) {
        show_error(QObject::tr("Server info callback failure").toUtf8().constData());
        return;
    }

    w->updateServer(*i);
    dec_outstanding(w);
}

void ext_stream_restore_read_cb(
        pa_context *,
        const pa_ext_stream_restore_info *i,
        int eol,
        void *userdata) {

    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        dec_outstanding(w);
        g_debug(QObject::tr("Failed to initialize stream_restore extension: %s").toUtf8().constData(), pa_strerror(pa_context_errno(context)));
        w->deleteEventRoleWidget();
        return;
    }

    if (eol > 0) {
        dec_outstanding(w);
        return;
    }

    w->updateRole(*i);
}

static void ext_stream_restore_subscribe_cb(pa_context *c, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);
    pa_operation *o;

    if (!(o = pa_ext_stream_restore_read(c, ext_stream_restore_read_cb, w))) {
        show_error(QObject::tr("pa_ext_stream_restore_read() failed").toUtf8().constData());
        return;
    }

    pa_operation_unref(o);
}

#if HAVE_EXT_DEVICE_RESTORE_API
void ext_device_restore_read_cb(
        pa_context *,
        const pa_ext_device_restore_info *i,
        int eol,
        void *userdata) {

    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        dec_outstanding(w);
        g_debug(QObject::tr("Failed to initialize device restore extension: %s").toUtf8().constData(), pa_strerror(pa_context_errno(context)));
        return;
    }

    if (eol > 0) {
        dec_outstanding(w);
        return;
    }

    /* Do something with a widget when this part is written */
    w->updateDeviceInfo(*i);
}

static void ext_device_restore_subscribe_cb(pa_context *c, pa_device_type_t type, uint32_t idx, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);
    pa_operation *o;

    if (type != PA_DEVICE_TYPE_SINK)
        return;

    if (!(o = pa_ext_device_restore_read_formats(c, type, idx, ext_device_restore_read_cb, w))) {
        show_error(QObject::tr("pa_ext_device_restore_read_sink_formats() failed").toUtf8().constData());
        return;
    }

    pa_operation_unref(o);
}
#endif

void ext_device_manager_read_cb(
        pa_context *,
        const pa_ext_device_manager_info *,
        int eol,
        void *userdata) {

    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        dec_outstanding(w);
        g_debug(QObject::tr("Failed to initialize device manager extension: %s").toUtf8().constData(), pa_strerror(pa_context_errno(context)));
        return;
    }

    w->canRenameDevices = true;

    if (eol > 0) {
        dec_outstanding(w);
        return;
    }

    /* Do something with a widget when this part is written */
}

static void ext_device_manager_subscribe_cb(pa_context *c, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);
    pa_operation *o;

    if (!(o = pa_ext_device_manager_read(c, ext_device_manager_read_cb, w))) {
        show_error(QObject::tr("pa_ext_device_manager_read() failed").toUtf8().constData());
        return;
    }

    pa_operation_unref(o);
}

void subscribe_cb(pa_context *c, pa_subscription_event_type_t t, uint32_t index, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
        case PA_SUBSCRIPTION_EVENT_SINK:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                w->removeSink(index);
            else {
                pa_operation *o;
                if (!(o = pa_context_get_sink_info_by_index(c, index, sink_cb, w))) {
                    show_error(QObject::tr("pa_context_get_sink_info_by_index() failed").toUtf8().constData());
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SOURCE:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                w->removeSource(index);
            else {
                pa_operation *o;
                if (!(o = pa_context_get_source_info_by_index(c, index, source_cb, w))) {
                    show_error(QObject::tr("pa_context_get_source_info_by_index() failed").toUtf8().constData());
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SINK_INPUT:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                w->removeSinkInput(index);
            else {
                pa_operation *o;
                if (!(o = pa_context_get_sink_input_info(c, index, sink_input_cb, w))) {
                    show_error(QObject::tr("pa_context_get_sink_input_info() failed").toUtf8().constData());
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                w->removeSourceOutput(index);
            else {
                pa_operation *o;
                if (!(o = pa_context_get_source_output_info(c, index, source_output_cb, w))) {
                    show_error(QObject::tr("pa_context_get_sink_input_info() failed").toUtf8().constData());
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_CLIENT:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                w->removeClient(index);
            else {
                pa_operation *o;
                if (!(o = pa_context_get_client_info(c, index, client_cb, w))) {
                    show_error(QObject::tr("pa_context_get_client_info() failed").toUtf8().constData());
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SERVER: {
                pa_operation *o;
                if (!(o = pa_context_get_server_info(c, server_info_cb, w))) {
                    show_error(QObject::tr("pa_context_get_server_info() failed").toUtf8().constData());
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_CARD:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                w->removeCard(index);
            else {
                pa_operation *o;
                if (!(o = pa_context_get_card_info_by_index(c, index, card_cb, w))) {
                    show_error(QObject::tr("pa_context_get_card_info_by_index() failed").toUtf8().constData());
                    return;
                }
                pa_operation_unref(o);
            }
            break;

    }
}

/* Forward Declaration */
gboolean connect_to_pulse(gpointer userdata);

void context_state_callback(pa_context *c, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    g_assert(c);

    switch (pa_context_get_state(c)) {
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
            break;

        case PA_CONTEXT_READY: {
            pa_operation *o;

            reconnect_timeout = 1;

            /* Create event widget immediately so it's first in the list */
            w->createEventRoleWidget();

            pa_context_set_subscribe_callback(c, subscribe_cb, w);

            if (!(o = pa_context_subscribe(c, (pa_subscription_mask_t)
                                           (PA_SUBSCRIPTION_MASK_SINK|
                                            PA_SUBSCRIPTION_MASK_SOURCE|
                                            PA_SUBSCRIPTION_MASK_SINK_INPUT|
                                            PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT|
                                            PA_SUBSCRIPTION_MASK_CLIENT|
                                            PA_SUBSCRIPTION_MASK_SERVER|
                                            PA_SUBSCRIPTION_MASK_CARD), nullptr, nullptr))) {
                show_error(QObject::tr("pa_context_subscribe() failed").toUtf8().constData());
                return;
            }
            pa_operation_unref(o);

            /* Keep track of the outstanding callbacks for UI tweaks */
            n_outstanding = 0;

            if (!(o = pa_context_get_server_info(c, server_info_cb, w))) {
                show_error(QObject::tr("pa_context_get_server_info() failed").toUtf8().constData());
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_client_info_list(c, client_cb, w))) {
                show_error(QObject::tr("pa_context_client_info_list() failed").toUtf8().constData());
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_card_info_list(c, card_cb, w))) {
                show_error(QObject::tr("pa_context_get_card_info_list() failed").toUtf8().constData());
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_sink_info_list(c, sink_cb, w))) {
                show_error(QObject::tr("pa_context_get_sink_info_list() failed").toUtf8().constData());
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_source_info_list(c, source_cb, w))) {
                show_error(QObject::tr("pa_context_get_source_info_list() failed").toUtf8().constData());
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_sink_input_info_list(c, sink_input_cb, w))) {
                show_error(QObject::tr("pa_context_get_sink_input_info_list() failed").toUtf8().constData());
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_source_output_info_list(c, source_output_cb, w))) {
                show_error(QObject::tr("pa_context_get_source_output_info_list() failed").toUtf8().constData());
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            /* These calls are not always supported */
            if ((o = pa_ext_stream_restore_read(c, ext_stream_restore_read_cb, w))) {
                pa_operation_unref(o);
                n_outstanding++;

                pa_ext_stream_restore_set_subscribe_cb(c, ext_stream_restore_subscribe_cb, w);

                if ((o = pa_ext_stream_restore_subscribe(c, 1, nullptr, nullptr)))
                    pa_operation_unref(o);

            } else
                g_debug(QObject::tr("Failed to initialize stream_restore extension: %s").toUtf8().constData(), pa_strerror(pa_context_errno(context)));

#if HAVE_EXT_DEVICE_RESTORE_API
            /* TODO Change this to just the test function */
            if ((o = pa_ext_device_restore_read_formats_all(c, ext_device_restore_read_cb, w))) {
                pa_operation_unref(o);
                n_outstanding++;

                pa_ext_device_restore_set_subscribe_cb(c, ext_device_restore_subscribe_cb, w);

                if ((o = pa_ext_device_restore_subscribe(c, 1, nullptr, nullptr)))
                    pa_operation_unref(o);

            } else
                g_debug(QObject::tr("Failed to initialize device restore extension: %s").toUtf8().constData(), pa_strerror(pa_context_errno(context)));
#endif

            if ((o = pa_ext_device_manager_read(c, ext_device_manager_read_cb, w))) {
                pa_operation_unref(o);
                n_outstanding++;

                pa_ext_device_manager_set_subscribe_cb(c, ext_device_manager_subscribe_cb, w);

                if ((o = pa_ext_device_manager_subscribe(c, 1, nullptr, nullptr)))
                    pa_operation_unref(o);

            } else
                g_debug(QObject::tr("Failed to initialize device manager extension: %s").toUtf8().constData(), pa_strerror(pa_context_errno(context)));


            break;
        }

        case PA_CONTEXT_FAILED:
            w->setConnectionState(false);

            w->removeAllWidgets();
            w->updateDeviceVisibility();
            pa_context_unref(context);
            context = nullptr;

            if (reconnect_timeout > 0) {
                g_debug("%s", QObject::tr("Connection failed, attempting reconnect").toUtf8().constData());
                g_timeout_add_seconds(reconnect_timeout, connect_to_pulse, w);
            }
            return;

        case PA_CONTEXT_TERMINATED:
        default:
            qApp->quit();
            return;
    }
}

pa_context* get_context() {
  return context;
}

gboolean connect_to_pulse(gpointer userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (context)
        return false;

    pa_proplist *proplist = pa_proplist_new();
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_NAME, QObject::tr("PulseAudio Volume Control").toUtf8().constData());
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_ID, "org.PulseAudio.pavucontrol");
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_ICON_NAME, "audio-card");
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_VERSION, PACKAGE_VERSION);

    context = pa_context_new_with_proplist(api, nullptr, proplist);
    g_assert(context);

    pa_proplist_free(proplist);

    pa_context_set_state_callback(context, context_state_callback, w);

    w->setConnectingMessage();
    if (pa_context_connect(context, nullptr, PA_CONTEXT_NOFAIL, nullptr) < 0) {
        if (pa_context_errno(context) == PA_ERR_INVALID) {
            w->setConnectingMessage(QObject::tr("Connection to PulseAudio failed. Automatic retry in 5s.<br><br>"
                "In this case this is likely because PULSE_SERVER in the Environment/X11 Root Window Properties"
                "or default-server in client.conf is misconfigured.<br>"
                "This situation can also arrise when PulseAudio crashed and left stale details in the X11 Root Window.<br>"
                "If this is the case, then PulseAudio should autospawn again, or if this is not configured you should"
                "run start-pulseaudio-x11 manually.").toUtf8().constData());
            reconnect_timeout = 5;
        }
        else {
            if(!retry) {
                reconnect_timeout = -1;
                qApp->quit();
            } else {
                g_debug("%s", QObject::tr("Connection failed, attempting reconnect").toUtf8().constData());
                reconnect_timeout = 5;
                g_timeout_add_seconds(reconnect_timeout, connect_to_pulse, userdata);
            }
        }
    }

    return false;
}

int main(int argc, char *argv[]) {

    signal(SIGPIPE, SIG_IGN);

    QApplication app(argc, argv);

    app.setOrganizationName(QStringLiteral("pavucontrol-qt"));

    QString locale = QLocale::system().name();
    QTranslator qtTranslator;
    if(qtTranslator.load(QStringLiteral("qt_") + locale, QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
        qApp->installTranslator(&qtTranslator);

    QTranslator appTranslator;
    if(appTranslator.load(QStringLiteral("pavucontrol-qt_") + locale, QStringLiteral(PAVUCONTROL_QT_DATA_DIR) + QStringLiteral("/translations")))
        qApp->installTranslator(&appTranslator);

    QCommandLineParser parser;
    parser.setApplicationDescription(QObject::tr("PulseAudio Volume Control"));
    parser.addHelpOption();

    const QString VERINFO = QStringLiteral(PAVUCONTROLQT_VERSION
                                           "\nQt        " QT_VERSION_STR);
    app.setApplicationVersion(VERINFO);
    parser.addVersionOption();

    QCommandLineOption tabOption(QStringList() << QStringLiteral("tab") << QStringLiteral("t"), QObject::tr("Select a specific tab on load."), QStringLiteral("tab"));
    parser.addOption(tabOption);

    QCommandLineOption retryOption(QStringList() << QStringLiteral("retry") << QStringLiteral("r"), QObject::tr("Retry forever if pa quits (every 5 seconds)."));
    parser.addOption(retryOption);

    QCommandLineOption maximizeOption(QStringList() << QStringLiteral("maximize") << QStringLiteral("m"), QObject::tr("Maximize the window."));
    parser.addOption(maximizeOption);

    parser.process(app);
    default_tab = parser.value(tabOption).toInt();
    retry = parser.isSet(retryOption);

    // ca_context_set_driver(ca_gtk_context_get(), "pulse");

    MainWindow* mainWindow = new MainWindow();
    if(parser.isSet(maximizeOption))
        mainWindow->showMaximized();

    pa_glib_mainloop *m = pa_glib_mainloop_new(g_main_context_default());
    g_assert(m);
    api = pa_glib_mainloop_get_api(m);
    g_assert(api);

    connect_to_pulse(mainWindow);
    if (reconnect_timeout >= 0) {
        mainWindow->show();
        app.exec();
    }

    if (reconnect_timeout < 0)
        show_error(QObject::tr("Fatal Error: Unable to connect to PulseAudio").toUtf8().constData());

    delete mainWindow;

    if (context)
        pa_context_unref(context);
    pa_glib_mainloop_free(m);

    return 0;
}
