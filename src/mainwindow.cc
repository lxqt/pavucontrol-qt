/***
  This file is part of pavucontrol.

  Copyright 2006-2008 Lennart Poettering
  Copyright 2009 Colin Guthrie

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

#include <set>

#include "mainwindow.h"

#include "cardwidget.h"
#include "sinkwidget.h"
#include "sourcewidget.h"
#include "sinkinputwidget.h"
#include "sourceoutputwidget.h"
#include "rolewidget.h"

#include "i18n.h"

/* Used for profile sorting */
struct profile_prio_compare {
    bool operator() (const pa_card_profile_info& lhs, const pa_card_profile_info& rhs) const {

        if (lhs.priority == rhs.priority)
            return strcmp(lhs.name, rhs.name) > 0;

        return lhs.priority > rhs.priority;
    }
};


MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    Gtk::Window(cobject),
    showSinkInputType(SINK_INPUT_CLIENT),
    showSinkType(SINK_ALL),
    showSourceOutputType(SOURCE_OUTPUT_CLIENT),
    showSourceType(SOURCE_NO_MONITOR),
    eventRoleWidget(NULL){

    x->get_widget("cardsVBox", cardsVBox);
    x->get_widget("streamsVBox", streamsVBox);
    x->get_widget("recsVBox", recsVBox);
    x->get_widget("sinksVBox", sinksVBox);
    x->get_widget("sourcesVBox", sourcesVBox);
    x->get_widget("noCardsLabel", noCardsLabel);
    x->get_widget("noStreamsLabel", noStreamsLabel);
    x->get_widget("noRecsLabel", noRecsLabel);
    x->get_widget("noSinksLabel", noSinksLabel);
    x->get_widget("noSourcesLabel", noSourcesLabel);
    x->get_widget("sinkInputTypeComboBox", sinkInputTypeComboBox);
    x->get_widget("sourceOutputTypeComboBox", sourceOutputTypeComboBox);
    x->get_widget("sinkTypeComboBox", sinkTypeComboBox);
    x->get_widget("sourceTypeComboBox", sourceTypeComboBox);
    x->get_widget("notebook", notebook);

    cardsVBox->set_reallocate_redraws(true);
    sourcesVBox->set_reallocate_redraws(true);
    streamsVBox->set_reallocate_redraws(true);
    recsVBox->set_reallocate_redraws(true);
    sinksVBox->set_reallocate_redraws(true);

    sinkInputTypeComboBox->set_active((int) showSinkInputType);
    sourceOutputTypeComboBox->set_active((int) showSourceOutputType);
    sinkTypeComboBox->set_active((int) showSinkType);
    sourceTypeComboBox->set_active((int) showSourceType);

    sinkInputTypeComboBox->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::onSinkInputTypeComboBoxChanged));
    sourceOutputTypeComboBox->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::onSourceOutputTypeComboBoxChanged));
    sinkTypeComboBox->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::onSinkTypeComboBoxChanged));
    sourceTypeComboBox->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::onSourceTypeComboBoxChanged));

    sinkTree = Gtk::ListStore::create(deviceColumns);
    sourceTree = Gtk::ListStore::create(deviceColumns);
}

MainWindow* MainWindow::create() {
    MainWindow* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "mainWindow");
    x->get_widget_derived("mainWindow", w);
    return w;
}

void MainWindow::on_realize() {
    Gtk::Window::on_realize();

    get_window()->set_cursor(Gdk::Cursor(Gdk::WATCH));
}

MainWindow::~MainWindow() {
    while (!clientNames.empty()) {
        std::map<uint32_t, char*>::iterator i = clientNames.begin();
        g_free(i->second);
        clientNames.erase(i);
    }
}

static void set_icon_name_fallback(Gtk::Image *i, const char *name, Gtk::IconSize size) {
    Glib::RefPtr<Gtk::IconTheme> theme;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;
    gint width = 24, height = 24;

    Gtk::IconSize::lookup(size, width, height);
    theme = Gtk::IconTheme::get_default();

    try {
        pixbuf = theme->load_icon(name, width, Gtk::ICON_LOOKUP_GENERIC_FALLBACK);

        if (pixbuf)
            i->set(pixbuf);
        else
            i->set(name);
    } catch (Gtk::IconThemeError &e) {
        i->set(name);
    }
}

void MainWindow::updateCard(const pa_card_info &info) {
    CardWidget *w;
    bool is_new = false;
    const char *description, *icon;
    std::set<pa_card_profile_info,profile_prio_compare> profile_priorities;

    if (cardWidgets.count(info.index))
        w = cardWidgets[info.index];
    else {
        cardWidgets[info.index] = w = CardWidget::create();
        cardsVBox->pack_start(*w, false, false, 0);
        w->index = info.index;
        is_new = true;
    }

    w->updating = true;

    description = pa_proplist_gets(info.proplist, PA_PROP_DEVICE_DESCRIPTION);
    w->name = description ? description : info.name;
    w->nameLabel->set_markup(w->name.c_str());

    icon = pa_proplist_gets(info.proplist, PA_PROP_DEVICE_ICON_NAME);
    set_icon_name_fallback(w->iconImage, icon ? icon : "audio-card", Gtk::ICON_SIZE_SMALL_TOOLBAR);

    w->hasSinks = w->hasSources = false;
    profile_priorities.clear();
    for (uint32_t i=0; i<info.n_profiles; ++i) {
        w->hasSinks = w->hasSinks || (info.profiles[i].n_sinks > 0);
        w->hasSources = w->hasSources || (info.profiles[i].n_sources > 0);
        profile_priorities.insert(info.profiles[i]);
    }

    w->profiles.clear();

    for (std::set<pa_card_profile_info>::iterator i = profile_priorities.begin(); i != profile_priorities.end(); ++i)
        w->profiles.push_back(std::pair<Glib::ustring,Glib::ustring>(i->name, i->description));

    w->activeProfile = info.active_profile->name;

    w->updating = false;

    w->prepareMenu();

    if (is_new)
        updateDeviceVisibility();
}

void MainWindow::rebuildSinkCombo() {
    //int active_idx = -1;
    Gtk::TreeModel::Row row;

    // Update our sinkTree
    sinkTree->clear();
    row = *(sinkTree->append());
    row[deviceColumns.index] = -1;
    row[deviceColumns.name] = "Default Output";

    for (std::map<uint32_t, SinkWidget*>::iterator i = sinkWidgets.begin(); i != sinkWidgets.end(); ++i) {
        Gtk::TreeModel::Row row = *(sinkTree->append());
        row[deviceColumns.index] = i->first;
        row[deviceColumns.name] = i->second->description.c_str();
        /*if (i->first == activeProfile)
          active_idx = idx;
        idx++;*/
    }

    /*if (active_idx >= 0)
        sinkTree->set_active(active_idx);*/
}

void MainWindow::updateSink(const pa_sink_info &info) {
    SinkWidget *w;
    bool is_new = false;
    const char *icon;

    if (sinkWidgets.count(info.index))
        w = sinkWidgets[info.index];
    else {
        sinkWidgets[info.index] = w = SinkWidget::create();
        w->beepDevice = info.name;
        w->setChannelMap(info.channel_map, !!(info.flags & PA_SINK_DECIBEL_VOLUME));
        sinksVBox->pack_start(*w, false, false, 0);
        w->index = info.index;
        w->monitor_index = info.monitor_source;
        is_new = true;

        w->setBaseVolume(info.base_volume);
        w->setSteps(info.n_volume_steps);
    }

    w->updating = true;

    w->card_index = info.card;
    w->name = info.name;
    w->description = info.description;
    w->type = info.flags & PA_SINK_HARDWARE ? SINK_HARDWARE : SINK_VIRTUAL;

    w->boldNameLabel->set_text("");
    gchar *txt;
    w->nameLabel->set_markup(txt = g_markup_printf_escaped("%s", info.description));
    g_free(txt);

    icon = pa_proplist_gets(info.proplist, PA_PROP_DEVICE_ICON_NAME);
    set_icon_name_fallback(w->iconImage, icon ? icon : "audio-card", Gtk::ICON_SIZE_SMALL_TOOLBAR);

    w->setVolume(info.volume);
    w->muteToggleButton->set_active(info.mute);

    w->defaultMenuItem.set_active(w->name == defaultSinkName);

    w->updating = false;

    rebuildSinkCombo();
    if (is_new)
        updateDeviceVisibility();
}

static void suspended_callback(pa_stream *s, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (pa_stream_is_suspended(s))
        w->updateVolumeMeter(pa_stream_get_device_index(s), PA_INVALID_INDEX, -1);
}

static void read_callback(pa_stream *s, size_t length, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);
    const void *data;
    double v;

    if (pa_stream_peek(s, &data, &length) < 0) {
        show_error(_("Failed to read data from stream"));
        return;
    }

    assert(length > 0);
    assert(length % sizeof(float) == 0);

    v = ((const float*) data)[length / sizeof(float) -1];

    pa_stream_drop(s);

    if (v < 0)
        v = 0;
    if (v > 1)
        v = 1;

    w->updateVolumeMeter(pa_stream_get_device_index(s), pa_stream_get_monitor_stream(s), v);
}

void MainWindow::createMonitorStreamForSource(uint32_t source_idx) {
    pa_stream *s;
    char t[16];
    pa_buffer_attr attr;
    pa_sample_spec ss;

    ss.channels = 1;
    ss.format = PA_SAMPLE_FLOAT32;
    ss.rate = 25;

    memset(&attr, 0, sizeof(attr));
    attr.fragsize = sizeof(float);
    attr.maxlength = (uint32_t) -1;

    snprintf(t, sizeof(t), "%u", source_idx);

    if (!(s = pa_stream_new(get_context(), _("Peak detect"), &ss, NULL))) {
        show_error(_("Failed to create monitoring stream"));
        return;
    }

    pa_stream_set_read_callback(s, read_callback, this);
    pa_stream_set_suspended_callback(s, suspended_callback, this);

    if (pa_stream_connect_record(s, t, &attr, (pa_stream_flags_t) (PA_STREAM_DONT_MOVE|PA_STREAM_PEAK_DETECT|PA_STREAM_ADJUST_LATENCY)) < 0) {
        show_error(_("Failed to connect monitoring stream"));
        pa_stream_unref(s);
        return;
    }
}

void MainWindow::createMonitorStreamForSinkInput(uint32_t sink_input_idx, uint32_t sink_idx) {
    pa_stream *s;
    char t[16];
    pa_buffer_attr attr;
    pa_sample_spec ss;
    uint32_t monitor_source_idx;

    ss.channels = 1;
    ss.format = PA_SAMPLE_FLOAT32;
    ss.rate = 25;

    if (!sinkWidgets.count(sink_idx))
        return;

    monitor_source_idx = sinkWidgets[sink_idx]->monitor_index;

    memset(&attr, 0, sizeof(attr));
    attr.fragsize = sizeof(float);
    attr.maxlength = (uint32_t) -1;

    snprintf(t, sizeof(t), "%u", monitor_source_idx);

    if (!(s = pa_stream_new(get_context(), _("Peak detect"), &ss, NULL))) {
        show_error(_("Failed to create monitoring stream"));
        return;
    }

    pa_stream_set_monitor_stream(s, sink_input_idx);
    pa_stream_set_read_callback(s, read_callback, this);
    pa_stream_set_suspended_callback(s, suspended_callback, this);

    if (pa_stream_connect_record(s, t, &attr, (pa_stream_flags_t) (PA_STREAM_DONT_MOVE|PA_STREAM_PEAK_DETECT|PA_STREAM_ADJUST_LATENCY)) < 0) {
        show_error(_("Failed to connect monitoring stream"));
        pa_stream_unref(s);
        return;
    }
}

void MainWindow::rebuildSourceCombo() {
    //int active_idx = -1;
    Gtk::TreeModel::Row row;

    // Update our sinkTree
    sourceTree->clear();
    row = *(sourceTree->append());
    row[deviceColumns.index] = -1;
    row[deviceColumns.name] = "Default Input";

    for (std::map<uint32_t, SourceWidget*>::iterator i = sourceWidgets.begin(); i != sourceWidgets.end(); ++i) {
        Gtk::TreeModel::Row row = *(sourceTree->append());
        row[deviceColumns.index] = i->first;
        row[deviceColumns.name] = i->second->description.c_str();
        /*if (i->first == activeProfile)
          active_idx = idx;
        idx++;*/
    }

    /*if (active_idx >= 0)
        sourceTree->set_active(active_idx);*/
}

void MainWindow::updateSource(const pa_source_info &info) {
    SourceWidget *w;
    bool is_new = false;
    const char *icon;

    if (sourceWidgets.count(info.index))
        w = sourceWidgets[info.index];
    else {
        sourceWidgets[info.index] = w = SourceWidget::create();
        w->setChannelMap(info.channel_map, !!(info.flags & PA_SOURCE_DECIBEL_VOLUME));
        sourcesVBox->pack_start(*w, false, false, 0);
        w->index = info.index;
        is_new = true;

        w->setBaseVolume(info.base_volume);
        w->setSteps(info.n_volume_steps);

        if (pa_context_get_server_protocol_version(get_context()) >= 13)
            createMonitorStreamForSource(info.index);
    }

    w->updating = true;

    w->card_index = info.card;
    w->name = info.name;
    w->description = info.description;
    w->type = info.monitor_of_sink != PA_INVALID_INDEX ? SOURCE_MONITOR : (info.flags & PA_SOURCE_HARDWARE ? SOURCE_HARDWARE : SOURCE_VIRTUAL);

    w->boldNameLabel->set_text("");
    gchar *txt;
    w->nameLabel->set_markup(txt = g_markup_printf_escaped("%s", info.description));
    g_free(txt);

    icon = pa_proplist_gets(info.proplist, PA_PROP_DEVICE_ICON_NAME);
    set_icon_name_fallback(w->iconImage, icon ? icon : "audio-input-microphone", Gtk::ICON_SIZE_SMALL_TOOLBAR);

    w->setVolume(info.volume);
    w->muteToggleButton->set_active(info.mute);

    w->defaultMenuItem.set_active(w->name == defaultSourceName);

    w->updating = false;

    rebuildSourceCombo();
    if (is_new)
        updateDeviceVisibility();
}

void MainWindow::setIconFromProplist(Gtk::Image *icon, pa_proplist *l, const char *def) {
    const char *t;

    if ((t = pa_proplist_gets(l, PA_PROP_MEDIA_ICON_NAME)))
        goto finish;

    if ((t = pa_proplist_gets(l, PA_PROP_WINDOW_ICON_NAME)))
        goto finish;

    if ((t = pa_proplist_gets(l, PA_PROP_APPLICATION_ICON_NAME)))
        goto finish;

    if ((t = pa_proplist_gets(l, PA_PROP_MEDIA_ROLE))) {

        if (strcmp(t, "video") == 0 ||
            strcmp(t, "phone") == 0)
            goto finish;

        if (strcmp(t, "music") == 0) {
            t = "audio";
            goto finish;
        }

        if (strcmp(t, "game") == 0) {
            t = "applications-games";
            goto finish;
        }

        if (strcmp(t, "event") == 0) {
            t = "dialog-information";
            goto finish;
        }
    }

    t = def;

finish:

    icon->set_from_icon_name(t, Gtk::ICON_SIZE_SMALL_TOOLBAR);
}

void MainWindow::updateSinkInput(const pa_sink_input_info &info) {
    SinkInputWidget *w;
    bool is_new = false;

    if (sinkInputWidgets.count(info.index))
        w = sinkInputWidgets[info.index];
    else {
        sinkInputWidgets[info.index] = w = SinkInputWidget::create(this);
        w->setChannelMap(info.channel_map, true);
        streamsVBox->pack_start(*w, false, false, 0);
        w->index = info.index;
        w->clientIndex = info.client;
        is_new = true;

        if (pa_context_get_server_protocol_version(get_context()) >= 13)
            createMonitorStreamForSinkInput(info.index, info.sink);
    }

    w->updating = true;

    w->type = info.client != PA_INVALID_INDEX ? SINK_INPUT_CLIENT : SINK_INPUT_VIRTUAL;

    w->sinkIndex = info.sink;

    char *txt;
    if (clientNames.count(info.client)) {
        w->boldNameLabel->set_markup(txt = g_markup_printf_escaped("<b>%s</b>", clientNames[info.client]));
        g_free(txt);
        w->nameLabel->set_markup(txt = g_markup_printf_escaped(": %s", info.name));
        g_free(txt);
    } else {
        w->boldNameLabel->set_text("");
        w->nameLabel->set_label(info.name);
    }

    setIconFromProplist(w->iconImage, info.proplist, "audio-card");

    w->setVolume(info.volume);
    w->muteToggleButton->set_active(info.mute);

    w->updating = false;

    if (is_new)
        updateDeviceVisibility();
}

void MainWindow::updateSourceOutput(const pa_source_output_info &info) {
    SourceOutputWidget *w;
    const char *app;
    bool is_new = false;

    if ((app = pa_proplist_gets(info.proplist, PA_PROP_APPLICATION_ID)))
        if (strcmp(app, "org.PulseAudio.pavucontrol") == 0)
            return;

    if (sourceOutputWidgets.count(info.index))
        w = sourceOutputWidgets[info.index];
    else {
        sourceOutputWidgets[info.index] = w = SourceOutputWidget::create(this);
        recsVBox->pack_start(*w, false, false, 0);
        w->index = info.index;
        w->clientIndex = info.client;
    }

    w->updating = true;

    w->type = info.client != PA_INVALID_INDEX ? SOURCE_OUTPUT_CLIENT : SOURCE_OUTPUT_VIRTUAL;

    w->sourceIndex = info.source;

    char *txt;
    if (clientNames.count(info.client)) {
        w->boldNameLabel->set_markup(txt = g_markup_printf_escaped("<b>%s</b>", clientNames[info.client]));
        g_free(txt);
        w->nameLabel->set_markup(txt = g_markup_printf_escaped(": %s", info.name));
        g_free(txt);
    } else {
        w->boldNameLabel->set_text("");
        w->nameLabel->set_label(info.name);
    }

    setIconFromProplist(w->iconImage, info.proplist, "audio-input-microphone");

    w->updating = false;

    if (is_new)
        updateDeviceVisibility();
}

void MainWindow::updateClient(const pa_client_info &info) {

    g_free(clientNames[info.index]);
    clientNames[info.index] = g_strdup(info.name);

    for (std::map<uint32_t, SinkInputWidget*>::iterator i = sinkInputWidgets.begin(); i != sinkInputWidgets.end(); ++i) {
        SinkInputWidget *w = i->second;

        if (!w)
            continue;

        if (w->clientIndex == info.index) {
            gchar *txt;
            w->boldNameLabel->set_markup(txt = g_markup_printf_escaped("<b>%s</b>", info.name));
            g_free(txt);
        }
    }
}

void MainWindow::updateServer(const pa_server_info &info) {

    defaultSourceName = info.default_source_name ? info.default_source_name : "";
    defaultSinkName = info.default_sink_name ? info.default_sink_name : "";

    for (std::map<uint32_t, SinkWidget*>::iterator i = sinkWidgets.begin(); i != sinkWidgets.end(); ++i) {
        SinkWidget *w = i->second;

        if (!w)
            continue;

        w->updating = true;
        w->defaultMenuItem.set_active(w->name == defaultSinkName);
        w->updating = false;
    }

    for (std::map<uint32_t, SourceWidget*>::iterator i = sourceWidgets.begin(); i != sourceWidgets.end(); ++i) {
        SourceWidget *w = i->second;

        if (!w)
            continue;

        w->updating = true;
        w->defaultMenuItem.set_active(w->name == defaultSourceName);
        w->updating = false;
    }
}

bool MainWindow::createEventRoleWidget() {
    if (eventRoleWidget)
        return FALSE;

    pa_channel_map cm = {
        1, { PA_CHANNEL_POSITION_MONO }
    };

    eventRoleWidget = RoleWidget::create();
    streamsVBox->pack_start(*eventRoleWidget, false, false, 0);
    eventRoleWidget->role = "sink-input-by-media-role:event";
    eventRoleWidget->setChannelMap(cm, true);

    eventRoleWidget->boldNameLabel->set_text("");
    eventRoleWidget->nameLabel->set_label(_("System Sounds"));

    eventRoleWidget->iconImage->set_from_icon_name("multimedia-volume-control", Gtk::ICON_SIZE_SMALL_TOOLBAR);

    eventRoleWidget->device = "";

    eventRoleWidget->updating = true;

    pa_cvolume volume;
    volume.channels = 1;
    volume.values[0] = PA_VOLUME_NORM;

    eventRoleWidget->setVolume(volume);
    eventRoleWidget->muteToggleButton->set_active(false);

    eventRoleWidget->updating = false;

    return TRUE;
}

void MainWindow::deleteEventRoleWidget() {

    if (eventRoleWidget)
        delete eventRoleWidget;

    eventRoleWidget = NULL;
}

void MainWindow::updateRole(const pa_ext_stream_restore_info &info) {
    pa_cvolume volume;
    bool is_new = false;

    if (strcmp(info.name, "sink-input-by-media-role:event") != 0)
        return;

    is_new = createEventRoleWidget();

    eventRoleWidget->updating = true;

    eventRoleWidget->device = info.device ? info.device : "";

    volume.channels = 1;
    volume.values[0] = pa_cvolume_max(&info.volume);

    eventRoleWidget->setVolume(volume);
    eventRoleWidget->muteToggleButton->set_active(info.mute);

    eventRoleWidget->updating = false;

    if (is_new)
        updateDeviceVisibility();
}

void MainWindow::updateVolumeMeter(uint32_t source_index, uint32_t sink_input_idx, double v) {

    if (sink_input_idx != PA_INVALID_INDEX) {
        SinkInputWidget *w;

        if (sinkInputWidgets.count(sink_input_idx)) {
            w = sinkInputWidgets[sink_input_idx];
            w->updatePeak(v);
        }

    } else {

        for (std::map<uint32_t, SinkWidget*>::iterator i = sinkWidgets.begin(); i != sinkWidgets.end(); ++i) {
            SinkWidget* w = i->second;

            if (w->monitor_index == source_index)
                w->updatePeak(v);
        }

        for (std::map<uint32_t, SourceWidget*>::iterator i = sourceWidgets.begin(); i != sourceWidgets.end(); ++i) {
            SourceWidget* w = i->second;

            if (w->index == source_index)
                w->updatePeak(v);
        }

        for (std::map<uint32_t, SourceOutputWidget*>::iterator i = sourceOutputWidgets.begin(); i != sourceOutputWidgets.end(); ++i) {
            SourceOutputWidget* w = i->second;

            if (w->sourceIndex == source_index)
                w->updatePeak(v);
        }
    }
}

static guint idle_source = 0;

gboolean idle_cb(gpointer data) {
    ((MainWindow*) data)->reallyUpdateDeviceVisibility();
    idle_source = 0;
    return FALSE;
}

void MainWindow::updateDeviceVisibility() {

    if (idle_source)
        return;

    idle_source = g_idle_add(idle_cb, this);
}

void MainWindow::reallyUpdateDeviceVisibility() {
    bool is_empty = true;

    for (std::map<uint32_t, SinkInputWidget*>::iterator i = sinkInputWidgets.begin(); i != sinkInputWidgets.end(); ++i) {
        SinkInputWidget* w = i->second;

        if (showSinkInputType == SINK_INPUT_ALL || w->type == showSinkInputType) {
            w->show();
            is_empty = false;
        } else
            w->hide();
    }

    if (eventRoleWidget)
        is_empty = false;

    if (is_empty)
        noStreamsLabel->show();
    else
        noStreamsLabel->hide();

    is_empty = true;

    for (std::map<uint32_t, SourceOutputWidget*>::iterator i = sourceOutputWidgets.begin(); i != sourceOutputWidgets.end(); ++i) {
        SourceOutputWidget* w = i->second;

        if (showSourceOutputType == SOURCE_OUTPUT_ALL || w->type == showSourceOutputType) {
            w->show();
            is_empty = false;
        } else
            w->hide();
    }

    if (is_empty)
        noRecsLabel->show();
    else
        noRecsLabel->hide();

    is_empty = true;

    for (std::map<uint32_t, SinkWidget*>::iterator i = sinkWidgets.begin(); i != sinkWidgets.end(); ++i) {
        SinkWidget* w = i->second;

        if (showSinkType == SINK_ALL || w->type == showSinkType) {
            w->show();
            is_empty = false;
        } else
            w->hide();
    }

    if (is_empty)
        noSinksLabel->show();
    else
        noSinksLabel->hide();

    is_empty = true;

    for (std::map<uint32_t, CardWidget*>::iterator i = cardWidgets.begin(); i != cardWidgets.end(); ++i) {
        CardWidget* w = i->second;

        w->show();
        is_empty = false;
    }

    if (is_empty)
        noCardsLabel->show();
    else
        noCardsLabel->hide();

    is_empty = true;

    for (std::map<uint32_t, SourceWidget*>::iterator i = sourceWidgets.begin(); i != sourceWidgets.end(); ++i) {
        SourceWidget* w = i->second;

        if (showSourceType == SOURCE_ALL ||
            w->type == showSourceType ||
            (showSourceType == SOURCE_NO_MONITOR && w->type != SOURCE_MONITOR)) {
            w->show();
            is_empty = false;
        } else
            w->hide();
    }

    if (is_empty)
        noSourcesLabel->show();
    else
        noSourcesLabel->hide();

    /* Hmm, if I don't call hide()/show() here some widgets will never
     * get their proper space allocated */
    sinksVBox->hide();
    sinksVBox->show();
    sourcesVBox->hide();
    sourcesVBox->show();
    streamsVBox->hide();
    streamsVBox->show();
    recsVBox->hide();
    recsVBox->show();
    cardsVBox->hide();
    cardsVBox->show();
}

void MainWindow::removeCard(uint32_t index) {
    if (!cardWidgets.count(index))
        return;

    delete cardWidgets[index];
    cardWidgets.erase(index);
    updateDeviceVisibility();
}

void MainWindow::removeSink(uint32_t index) {
    if (!sinkWidgets.count(index))
        return;

    delete sinkWidgets[index];
    sinkWidgets.erase(index);
    rebuildSinkCombo();
    updateDeviceVisibility();
}

void MainWindow::removeSource(uint32_t index) {
    if (!sourceWidgets.count(index))
        return;

    delete sourceWidgets[index];
    sourceWidgets.erase(index);
    rebuildSourceCombo();
    updateDeviceVisibility();
}

void MainWindow::removeSinkInput(uint32_t index) {
    if (!sinkInputWidgets.count(index))
        return;

    delete sinkInputWidgets[index];
    sinkInputWidgets.erase(index);
    updateDeviceVisibility();
}

void MainWindow::removeSourceOutput(uint32_t index) {
    if (!sourceOutputWidgets.count(index))
        return;

    delete sourceOutputWidgets[index];
    sourceOutputWidgets.erase(index);
    updateDeviceVisibility();
}

void MainWindow::removeClient(uint32_t index) {
    g_free(clientNames[index]);
    clientNames.erase(index);
}

void MainWindow::onSinkTypeComboBoxChanged() {
    showSinkType = (SinkType) sinkTypeComboBox->get_active_row_number();

    if (showSinkType == (SinkType) -1)
        sinkTypeComboBox->set_active((int) SINK_ALL);

    updateDeviceVisibility();
}

void MainWindow::onSourceTypeComboBoxChanged() {
    showSourceType = (SourceType) sourceTypeComboBox->get_active_row_number();

    if (showSourceType == (SourceType) -1)
        sourceTypeComboBox->set_active((int) SOURCE_NO_MONITOR);

    updateDeviceVisibility();
}

void MainWindow::onSinkInputTypeComboBoxChanged() {
    showSinkInputType = (SinkInputType) sinkInputTypeComboBox->get_active_row_number();

    if (showSinkInputType == (SinkInputType) -1)
        sinkInputTypeComboBox->set_active((int) SINK_INPUT_CLIENT);

    updateDeviceVisibility();
}

void MainWindow::onSourceOutputTypeComboBoxChanged() {
    showSourceOutputType = (SourceOutputType) sourceOutputTypeComboBox->get_active_row_number();

    if (showSourceOutputType == (SourceOutputType) -1)
        sourceOutputTypeComboBox->set_active((int) SOURCE_OUTPUT_CLIENT);

    updateDeviceVisibility();
}
