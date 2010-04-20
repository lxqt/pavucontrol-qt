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
  along with pavucontrol. If not, see <http://www.gnu.org/licenses/>.
***/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>
#include <pulse/ext-stream-restore.h>
#include <pulse/ext-device-manager.h>

#include <canberra-gtk.h>

#include "pavucontrol.h"
#include "i18n.h"
#include "minimalstreamwidget.h"
#include "channelwidget.h"
#include "streamwidget.h"
#include "cardwidget.h"
#include "sinkwidget.h"
#include "sourcewidget.h"
#include "sinkinputwidget.h"
#include "sourceoutputwidget.h"
#include "rolewidget.h"
#include "mainwindow.h"

static pa_context* context = NULL;
static pa_mainloop_api* api = NULL;
static int n_outstanding = 0;

void show_error(const char *txt) {
    char buf[256];

    snprintf(buf, sizeof(buf), "%s: %s", txt, pa_strerror(pa_context_errno(context)));

    Gtk::MessageDialog dialog(buf, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
    dialog.run();

    Gtk::Main::quit();
}

static void dec_outstanding(MainWindow *w) {
    if (n_outstanding <= 0)
        return;

    if (--n_outstanding <= 0)
        w->get_window()->set_cursor();
}

void card_cb(pa_context *, const pa_card_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        show_error(_("Card callback failure"));
        return;
    }

    if (eol > 0) {
        dec_outstanding(w);
        return;
    }

    w->updateCard(*i);
}

void sink_cb(pa_context *, const pa_sink_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        show_error(_("Sink callback failure"));
        return;
    }

    if (eol > 0) {
        dec_outstanding(w);
        return;
    }

    w->updateSink(*i);
}

void source_cb(pa_context *, const pa_source_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        show_error(_("Source callback failure"));
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

        show_error(_("Sink input callback failure"));
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

        show_error(_("Source output callback failure"));
        return;
    }

    if (eol > 0)  {

        if (n_outstanding > 0) {
            /* At this point all notebook pages have been populated, so
             * let's open one that isn't empty */

            if (w->sinkInputWidgets.size() > 0)
                w->notebook->set_current_page(0);
            else if (w->sourceOutputWidgets.size() > 0)
                w->notebook->set_current_page(1);
            else if (w->sourceWidgets.size() > 0 && w->sinkWidgets.size() == 0)
                w->notebook->set_current_page(3);
            else
                w->notebook->set_current_page(2);
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

        show_error(_("Client callback failure"));
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
        show_error(_("Server info callback failure"));
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
        g_debug(_("Failed to initialize stream_restore extension: %s"), pa_strerror(pa_context_errno(context)));
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
        show_error(_("pa_ext_stream_restore_read() failed"));
        return;
    }

    pa_operation_unref(o);
}

void ext_device_manager_read_cb(
        pa_context *,
        const pa_ext_device_manager_info *,
        int eol,
        void *userdata) {

    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol < 0) {
        dec_outstanding(w);
        g_debug(_("Failed to initialize device manager extension: %s"), pa_strerror(pa_context_errno(context)));
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
        show_error(_("pa_ext_device_manager_read() failed"));
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
                    show_error(_("pa_context_get_sink_info_by_index() failed"));
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
                    show_error(_("pa_context_get_source_info_by_index() failed"));
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
                    show_error(_("pa_context_get_sink_input_info() failed"));
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
                    show_error(_("pa_context_get_sink_input_info() failed"));
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
                    show_error(_("pa_context_get_client_info() failed"));
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SERVER: {
                pa_operation *o;
                if (!(o = pa_context_get_server_info(c, server_info_cb, w))) {
                    show_error(_("pa_context_get_server_info() failed"));
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
                    show_error(_("pa_context_get_card_info_by_index() failed"));
                    return;
                }
                pa_operation_unref(o);
            }
            break;

    }
}

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
                                            PA_SUBSCRIPTION_MASK_CARD), NULL, NULL))) {
                show_error(_("pa_context_subscribe() failed"));
                return;
            }
            pa_operation_unref(o);

            /* Keep track of the outstanding callbacks for UI tweaks */
            n_outstanding = 0;

            if (!(o = pa_context_get_server_info(c, server_info_cb, w))) {
                show_error(_("pa_context_get_server_info() failed"));
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_client_info_list(c, client_cb, w))) {
                show_error(_("pa_context_client_info_list() failed"));
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_card_info_list(c, card_cb, w))) {
                show_error(_("pa_context_get_card_info_list() failed"));
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_sink_info_list(c, sink_cb, w))) {
                show_error(_("pa_context_get_sink_info_list() failed"));
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_source_info_list(c, source_cb, w))) {
                show_error(_("pa_context_get_source_info_list() failed"));
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_sink_input_info_list(c, sink_input_cb, w))) {
                show_error(_("pa_context_get_sink_input_info_list() failed"));
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_source_output_info_list(c, source_output_cb, w))) {
                show_error(_("pa_context_get_source_output_info_list() failed"));
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            /* These calls are not always supported */
            if ((o = pa_ext_stream_restore_read(c, ext_stream_restore_read_cb, w))) {
                pa_operation_unref(o);
                n_outstanding++;

                pa_ext_stream_restore_set_subscribe_cb(c, ext_stream_restore_subscribe_cb, w);

                if ((o = pa_ext_stream_restore_subscribe(c, 1, NULL, NULL)))
                    pa_operation_unref(o);

            } else
                g_debug(_("Failed to initialize stream_restore extension: %s"), pa_strerror(pa_context_errno(context)));

            if ((o = pa_ext_device_manager_read(c, ext_device_manager_read_cb, w))) {
                pa_operation_unref(o);
                n_outstanding++;

                pa_ext_device_manager_set_subscribe_cb(c, ext_device_manager_subscribe_cb, w);

                if ((o = pa_ext_device_manager_subscribe(c, 1, NULL, NULL)))
                    pa_operation_unref(o);

            } else
                g_debug(_("Failed to initialize device manager extension: %s"), pa_strerror(pa_context_errno(context)));


            break;
        }

        case PA_CONTEXT_FAILED:
            show_error(_("Connection failed"));
            return;

        case PA_CONTEXT_TERMINATED:
        default:
            Gtk::Main::quit();
            return;
    }
}

static pa_context* create_context(MainWindow* w) {
    g_assert(api);

    pa_proplist *proplist = pa_proplist_new();
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_NAME, _("PulseAudio Volume Control"));
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_ID, "org.PulseAudio.pavucontrol");
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_ICON_NAME, "audio-card");
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_VERSION, PACKAGE_VERSION);

    pa_context* c = pa_context_new_with_proplist(api, NULL, proplist);
    g_assert(c);

    pa_proplist_free(proplist);

    pa_context_set_state_callback(c, context_state_callback, w);

    return c;
}

pa_context* get_context(void) {
  return context;
}

int main(int argc, char *argv[]) {

    /* Initialize the i18n stuff */
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);

    signal(SIGPIPE, SIG_IGN);

    Gtk::Main kit(argc, argv);

    ca_context_set_driver(ca_gtk_context_get(), "pulse");

    MainWindow* mainWindow = MainWindow::create();

    pa_glib_mainloop *m = pa_glib_mainloop_new(g_main_context_default());
    g_assert(m);
    api = pa_glib_mainloop_get_api(m);
    g_assert(api);

    context = create_context(mainWindow);

    if (pa_context_connect(context, NULL, (pa_context_flags_t) 0, NULL) < 0)
        goto finish;

    Gtk::Main::run(*mainWindow);
    delete mainWindow;

finish:
    pa_context_unref(context);
    pa_glib_mainloop_free(m);
}
