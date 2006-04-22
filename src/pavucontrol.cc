/* $Id$ */

/***
  This file is part of pavucontrol.
 
  pavucontrol is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation; either version 2 of the License,
  or (at your option) any later version.
 
  pavucontrol is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public License
  along with pavucontrol; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
***/

#include <signal.h>

#include <gtkmm.h>
#include <libglademm.h>

#include <polyp/polypaudio.h>
#include <polyp/glib-mainloop.h>

#ifndef GLADE_FILE
#define GLADE_FILE "pavucontrol.glade"
#endif

pa_context *context = NULL;

class StreamWidget;

class ChannelWidget : public Gtk::VBox {
public:
    ChannelWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static ChannelWidget* create();

    void setVolume(pa_volume_t volume);

    Gtk::Label *channelLabel;
    Gtk::Label *volumeLabel;
    Gtk::HScale *volumeScale;

    int channel;
    StreamWidget *streamWidget;

    void onVolumeScaleValueChanged();

    bool volumeScaleEnabled;

    virtual void set_sensitive(bool enabled);
};

class StreamWidget : public Gtk::VBox {
public:
    StreamWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);

    void setChannelMap(const pa_channel_map &m);
    void setVolume(const pa_cvolume &volume);
    virtual void updateChannelVolume(int channel, pa_volume_t v);

    Gtk::Label *nameLabel;
    Gtk::VBox *channelsVBox;
    Gtk::ToggleButton *lockToggleButton, *muteToggleButton;

    pa_channel_map channelMap;
    pa_cvolume volume;

    ChannelWidget *channelWidgets[PA_CHANNELS_MAX];

    virtual void onMuteToggleButton();
};

class SinkWidget : public StreamWidget {
public:
    SinkWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static SinkWidget* create();

    virtual void updateChannelVolume(int channel, pa_volume_t v);
    virtual void onMuteToggleButton();
    uint32_t index;
};

class SourceWidget : public StreamWidget {
public:
    SourceWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static SourceWidget* create();

    virtual void updateChannelVolume(int channel, pa_volume_t v);
    virtual void onMuteToggleButton();
    uint32_t index;
};

class SinkInputWidget : public StreamWidget {
public:
    SinkInputWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static SinkInputWidget* create();

    virtual void updateChannelVolume(int channel, pa_volume_t v);
    uint32_t index;
};

class MainWindow : public Gtk::Window {
public:
    MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static MainWindow* create();

    void updateSink(const pa_sink_info &info);
    void updateSource(const pa_source_info &info);
    void updateSinkInput(const pa_sink_input_info &info);

    void removeSink(uint32_t index);
    void removeSource(uint32_t index);
    void removeSinkInput(uint32_t index);
    
    Gtk::VBox *streamsVBox, *sinksVBox, *sourcesVBox, *monitorsVBox;
    Gtk::EventBox *titleEventBox;
    Gtk::Label *noStreamsLabel, *noSinksLabel, *noSourcesLabel, *noMonitorsLabel;

    std::map<int, SinkWidget*> sinkWidgets;
    std::map<int, SourceWidget*> sourceWidgets;
    std::map<int, SourceWidget*> monitorWidgets;
    std::map<int, SinkInputWidget*> streamWidgets;

    void updateLabels();
};

void show_error(const char *txt) {
    char buf[256];

    snprintf(buf, sizeof(buf), "%s: %s", txt, pa_strerror(pa_context_errno(context)));
    
    Gtk::MessageDialog dialog(buf, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
    dialog.run();

    Gtk::Main::quit();
}

/*** ChannelWidget ***/

ChannelWidget::ChannelWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    Gtk::VBox(cobject),
    volumeScaleEnabled(true) {

    x->get_widget("channelLabel", channelLabel);
    x->get_widget("volumeLabel", volumeLabel);
    x->get_widget("volumeScale", volumeScale);

    volumeScale->signal_value_changed().connect(sigc::mem_fun(*this, &ChannelWidget::onVolumeScaleValueChanged));
}

ChannelWidget* ChannelWidget::create() {
    ChannelWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "channelWidget");
    x->get_widget_derived("channelWidget", w);
    return w;
}

void ChannelWidget::setVolume(pa_volume_t volume) {
    double v = ((gdouble) volume * 100) / PA_VOLUME_NORM;
    char txt[64];
    
    snprintf(txt, sizeof(txt), "%0.0f%%", v);
    volumeLabel->set_text(txt);

    if (v > 100)
        v = 1000;

    volumeScaleEnabled = false;
    volumeScale->set_value(v);
    volumeScaleEnabled = true;
}

void ChannelWidget::onVolumeScaleValueChanged() {

    if (!volumeScaleEnabled)
        return;

    pa_volume_t volume = (pa_volume_t) ((volumeScale->get_value() * PA_VOLUME_NORM) / 100);
    streamWidget->updateChannelVolume(channel, volume);
}

void ChannelWidget::set_sensitive(bool enabled) {
    Gtk::VBox::set_sensitive(enabled);

    channelLabel->set_sensitive(enabled);
    volumeLabel->set_sensitive(enabled);
    volumeScale->set_sensitive(enabled);
}

/*** StreamWidget ***/

StreamWidget::StreamWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    Gtk::VBox(cobject) {

    x->get_widget("channelsVBox", channelsVBox);
    x->get_widget("nameLabel", nameLabel);
    x->get_widget("lockToggleButton", lockToggleButton);
    x->get_widget("muteToggleButton", muteToggleButton);

    muteToggleButton->signal_clicked().connect(sigc::mem_fun(*this, &StreamWidget::onMuteToggleButton));
        
    for (int i = 0; i < PA_CHANNELS_MAX; i++)
        channelWidgets[i] = NULL;
}

void StreamWidget::setChannelMap(const pa_channel_map &m) {
    channelMap = m;
    
    for (int i = 0; i < m.channels; i++) {
        ChannelWidget *cw = channelWidgets[i] = ChannelWidget::create();
        cw->channel = i;
        cw->streamWidget = this;
        char text[64];
        snprintf(text, sizeof(text), "<b>%s</b>", pa_channel_position_to_string(m.map[i]));
        cw->channelLabel->set_markup(text);
        channelsVBox->pack_start(*cw, false, false, 0);
    }

    lockToggleButton->set_sensitive(m.channels > 1);
}

void StreamWidget::setVolume(const pa_cvolume &v) {
    g_assert(v.channels == channelMap.channels);

    volume = v;
    
    for (int i = 0; i < volume.channels; i++)
        channelWidgets[i]->setVolume(volume.values[i]);
}

void StreamWidget::updateChannelVolume(int channel, pa_volume_t v) {
    g_assert(channel < volume.channels);

    if (lockToggleButton->get_active()) {
        for (int i = 0; i < volume.channels; i++)
            volume.values[i] = v;
    } else 
        volume.values[channel] = v;
}

void StreamWidget::onMuteToggleButton() {
    lockToggleButton->set_sensitive(!muteToggleButton->get_active());

    for (int i = 0; i < channelMap.channels; i++)
        channelWidgets[i]->set_sensitive(!muteToggleButton->get_active());
}

SinkWidget::SinkWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    StreamWidget(cobject, x) {
}

SinkWidget* SinkWidget::create() {
    SinkWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    return w;
}

void SinkWidget::updateChannelVolume(int channel, pa_volume_t v) {
    StreamWidget::updateChannelVolume(channel, v);
    
    pa_operation* o;
    if (!(o = pa_context_set_sink_volume_by_index(context, index, &volume, NULL, NULL))) {
        show_error("pa_context_set_sink_volume_by_index() failed");
        return;
    }

    pa_operation_unref(o);
}

void SinkWidget::onMuteToggleButton() {
    StreamWidget::onMuteToggleButton();

    pa_operation* o;
    if (!(o = pa_context_set_sink_mute_by_index(context, index, muteToggleButton->get_active(), NULL, NULL))) {
        show_error("pa_context_set_sink_mute_by_index() failed");
        return;
    }

    pa_operation_unref(o);
    
}

SourceWidget::SourceWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    StreamWidget(cobject, x) {
}

SourceWidget* SourceWidget::create() {
    SourceWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    return w;
}

void SourceWidget::updateChannelVolume(int channel, pa_volume_t v) {
    StreamWidget::updateChannelVolume(channel, v);

    pa_operation* o;
    if (!(o = pa_context_set_source_volume_by_index(context, index, &volume, NULL, NULL))) {
        show_error("pa_context_set_source_volume_by_index() failed");
        return;
    }

    pa_operation_unref(o);
    return;
}

void SourceWidget::onMuteToggleButton() {
    StreamWidget::onMuteToggleButton();
    
    pa_operation* o;
    if (!(o = pa_context_set_source_mute_by_index(context, index, muteToggleButton->get_active(), NULL, NULL))) {
        show_error("pa_context_set_source_mute_by_index() failed");
        return;
    }

    pa_operation_unref(o);
}

SinkInputWidget::SinkInputWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    StreamWidget(cobject, x) {
}

SinkInputWidget* SinkInputWidget::create() {
    SinkInputWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    return w;
}

void SinkInputWidget::updateChannelVolume(int channel, pa_volume_t v) {
    StreamWidget::updateChannelVolume(channel, v);

    pa_operation* o;
    if (!(o = pa_context_set_sink_input_volume(context, index, &volume, NULL, NULL))) {
        show_error("pa_context_set_sink_input_volume() failed");
        return;
    }

    pa_operation_unref(o);
}

/*** MainWindow ***/

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    Gtk::Window(cobject) {

    x->get_widget("streamsVBox", streamsVBox);
    x->get_widget("sinksVBox", sinksVBox);
    x->get_widget("sourcesVBox", sourcesVBox);
    x->get_widget("monitorsVBox", monitorsVBox);
    x->get_widget("titleEventBox", titleEventBox);
    x->get_widget("noStreamsLabel", noStreamsLabel);
    x->get_widget("noSinksLabel", noSinksLabel);
    x->get_widget("noSourcesLabel", noSourcesLabel);
    x->get_widget("noMonitorsLabel", noMonitorsLabel);

    sourcesVBox->set_reallocate_redraws(true);
    monitorsVBox->set_reallocate_redraws(true);
    
    Gdk::Color c("white");
    titleEventBox->modify_bg(Gtk::STATE_NORMAL, c);
}

MainWindow* MainWindow::create() {
    MainWindow* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "mainWindow");
    x->get_widget_derived("mainWindow", w);
    return w;
}

void MainWindow::updateSink(const pa_sink_info &info) {
    SinkWidget *w;

    if (sinkWidgets.count(info.index))
        w = sinkWidgets[info.index];
    else {
        sinkWidgets[info.index] = w = SinkWidget::create();
        w->setChannelMap(info.channel_map);
        sinksVBox->pack_start(*w, false, false, 0);
        w->index = info.index;
    }

    char txt[256];
    snprintf(txt, sizeof(txt), "<b>%s</b> - %s", info.name, info.description);
    w->nameLabel->set_markup(txt);
    w->setVolume(info.volume);
    w->muteToggleButton->set_active(info.mute);
    w->onMuteToggleButton();

    updateLabels();
    w->check_resize();
}

void MainWindow::updateSource(const pa_source_info &info) {
    SourceWidget *w;

    std::map<int, SourceWidget*> &l = info.monitor_of_sink != PA_INVALID_INDEX ? monitorWidgets : sourceWidgets;
    Gtk::VBox *vbox = info.monitor_of_sink != PA_INVALID_INDEX ? monitorsVBox : sourcesVBox;
    
    if (l.count(info.index))
        w = l[info.index];
    else {
        l[info.index] = w = SourceWidget::create();
        w->setChannelMap(info.channel_map);
        vbox->pack_start(*w, false, false, 0);
        w->index = info.index;
    }

    char txt[256];
    snprintf(txt, sizeof(txt), "<b>%s</b> - %s", info.name, info.description);
    w->nameLabel->set_markup(txt);
    w->setVolume(info.volume);
    w->muteToggleButton->set_active(info.mute);
    w->onMuteToggleButton();

    updateLabels();
    w->check_resize();
}

void MainWindow::updateSinkInput(const pa_sink_input_info &info) {
    SinkInputWidget *w;

    if (streamWidgets.count(info.index))
        w = streamWidgets[info.index];
    else {
        streamWidgets[info.index] = w = SinkInputWidget::create();
        w->setChannelMap(info.channel_map);
        streamsVBox->pack_start(*w, false, false, 0);
        w->muteToggleButton->hide();
        w->index = info.index;
    }

    char txt[256];
    snprintf(txt, sizeof(txt), "<b>%s</b>", info.name);
    w->nameLabel->set_markup(txt);
    w->setVolume(info.volume);

    updateLabels();
    w->check_resize();
}

void MainWindow::updateLabels() {
    if (streamWidgets.empty())
        noStreamsLabel->show();
    else
        noStreamsLabel->hide();
    
    if (sinkWidgets.empty())
        noSinksLabel->show();
    else
        noSinksLabel->hide();
    
    if (sourceWidgets.empty())
        noSourcesLabel->show();
    else
        noSourcesLabel->hide();

    if (monitorWidgets.empty())
        noMonitorsLabel->show();
    else
        noMonitorsLabel->hide();
}

void MainWindow::removeSink(uint32_t index) {
    StreamWidget *w;
    
    if (!(w = sinkWidgets[index]))
        return;

    sinkWidgets.erase(index);
    delete w;
    updateLabels();
}

void MainWindow::removeSource(uint32_t index) {
    StreamWidget *w;

    if (sourceWidgets.count(index)) {
        w = sourceWidgets[index];
        sourceWidgets.erase(index);
    } else if (monitorWidgets.count(index)) {
        w = monitorWidgets[index];
        monitorWidgets.erase(index);
    } else
        return;

    delete w;
    updateLabels();
}

void MainWindow::removeSinkInput(uint32_t index) {
    StreamWidget *w;
    
    if (!(w = streamWidgets[index]))
        return;

    streamWidgets.erase(index);
    delete w;
    updateLabels();
}

void sink_cb(pa_context *, const pa_sink_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol)
        return;

    if (!i) {
        show_error("Sink callback failure");
        return;
    }

    w->updateSink(*i);
}

void source_cb(pa_context *, const pa_source_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol)
        return;

    if (!i) {
        show_error("Source callback failure");
        return;
    }

    w->updateSource(*i);
}

void sink_input_cb(pa_context *, const pa_sink_input_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol)
        return;

    if (!i) {
        show_error("Sink input callback failure");
        return;
    }

    w->updateSinkInput(*i);
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
                    show_error("pa_context_get_sink_info_by_index() failed");
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
                    show_error("pa_context_get_source_info_by_index() failed");
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
                    show_error("pa_context_get_sink_input_info() failed");
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
            
            pa_context_set_subscribe_callback(c, subscribe_cb, w);
            
            if (!(o = pa_context_subscribe(c, (pa_subscription_mask_t) (PA_SUBSCRIPTION_MASK_SINK|PA_SUBSCRIPTION_MASK_SOURCE|PA_SUBSCRIPTION_MASK_SINK_INPUT), NULL, NULL))) {
                show_error("pa_context_subscribe() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_sink_info_list(c, sink_cb, w))) {
                show_error("pa_context_get_sink_info_list() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_source_info_list(c, source_cb, w))) {
                show_error("pa_context_get_source_info_list() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_sink_input_info_list(c, sink_input_cb, w))) {
                show_error("pa_context_get_sink_input_info_list() failed");
                return;
            }
            pa_operation_unref(o);
                
            break;
        }
            
        case PA_CONTEXT_FAILED:
            show_error("Connection failed");
            return;
            
        case PA_CONTEXT_TERMINATED:
        default:
            Gtk::Main::quit();
            return;
    }
}

int main(int argc, char *argv[]) {
    signal(SIGPIPE, SIG_IGN);

    Gtk::Main kit(argc, argv);
    
    Gtk::Window* mainWindow = MainWindow::create();
    
    pa_glib_mainloop *m = pa_glib_mainloop_new(g_main_context_default());
    g_assert(m);
    pa_mainloop_api *api = pa_glib_mainloop_get_api(m);
    g_assert(api);
    context = pa_context_new(api, "Polypaudio Volume Control");
    g_assert(context);

    pa_context_set_state_callback(context, context_state_callback, mainWindow);
    
    if (pa_context_connect(context, NULL, (pa_context_flags_t) 0, NULL) < 0) {
        show_error("Connection failed");
        goto finish;
    }
    
    Gtk::Main::run(*mainWindow);
    delete mainWindow;

finish:
    pa_context_unref(context);
    pa_glib_mainloop_free(m);
}
