/* $Id$ */

/***
  This file is part of pavucontrol.

  Copyright 2006-2008 Lennart Poettering <mzcnihpbageby (at) 0pointer (dot) de>
  Copyright 2008 Sjoerd Simons <sjoerd@luon.net>

  pavucontrol is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published
  by the Free Software Foundation; either version 2 of the License,
  or (at your option) any later version.

  pavucontrol is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with pavucontrol; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
***/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <signal.h>

#include <gtkmm.h>
#include <libglademm.h>

#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>

#ifndef GLADE_FILE
#define GLADE_FILE "pavucontrol.glade"
#endif

static pa_context *context = NULL;
static int n_outstanding = 0;
static bool show_decibel = true;

enum SinkInputType {
    SINK_INPUT_ALL,
    SINK_INPUT_CLIENT,
    SINK_INPUT_VIRTUAL
};

enum SinkType {
    SINK_ALL,
    SINK_HARDWARE,
    SINK_VIRTUAL,
};

enum SourceOutputType {
    SOURCE_OUTPUT_ALL,
    SOURCE_OUTPUT_CLIENT,
    SOURCE_OUTPUT_VIRTUAL
};

enum SourceType{
    SOURCE_ALL,
    SOURCE_NO_MONITOR,
    SOURCE_HARDWARE,
    SOURCE_VIRTUAL,
    SOURCE_MONITOR,
};

class StreamWidget;
class MainWindow;

class ChannelWidget : public Gtk::EventBox {
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

    bool can_decibel;
    bool volumeScaleEnabled;

    virtual void set_sensitive(bool enabled);
};

class MinimalStreamWidget : public Gtk::VBox {
public:
    MinimalStreamWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);

    Gtk::Label *nameLabel, *boldNameLabel;
    Gtk::ToggleButton *streamToggleButton;
    Gtk::Menu menu;

    bool updating;

    void onStreamToggleButton();
    void onMenuDeactivated();
    void popupMenuPosition(int& x, int& y, bool& push_in);

    virtual void prepareMenu(void);

protected:
    virtual bool on_button_press_event(GdkEventButton* event);
};

class StreamWidget : public MinimalStreamWidget {
public:
    StreamWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);

    void setChannelMap(const pa_channel_map &m, bool can_decibel);
    void setVolume(const pa_cvolume &volume, bool force);
    virtual void updateChannelVolume(int channel, pa_volume_t v);

    Gtk::VBox *channelsVBox;
    Gtk::ToggleButton *lockToggleButton, *muteToggleButton;

    pa_channel_map channelMap;
    pa_cvolume volume;

    ChannelWidget *channelWidgets[PA_CHANNELS_MAX];

    virtual void onMuteToggleButton();

    sigc::connection timeoutConnection;

    bool timeoutEvent();

    virtual void executeVolumeUpdate();
};

class SinkWidget : public StreamWidget {
public:
    SinkWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static SinkWidget* create();

    SinkType type;
    Glib::ustring description;
    Glib::ustring name;
    uint32_t index;
    bool can_decibel;

    Gtk::CheckMenuItem defaultMenuItem;

    virtual void onMuteToggleButton();
    virtual void executeVolumeUpdate();
    virtual void onDefaultToggle();
};

class SourceWidget : public StreamWidget {
public:
    SourceWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static SourceWidget* create();

    SourceType type;
    Glib::ustring name;
    Glib::ustring description;
    uint32_t index;
    bool can_decibel;

    Gtk::CheckMenuItem defaultMenuItem;

    virtual void onMuteToggleButton();
    virtual void executeVolumeUpdate();
    virtual void onDefaultToggle();
};

class SinkInputWidget : public StreamWidget {
public:
    SinkInputWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static SinkInputWidget* create();
    virtual ~SinkInputWidget();

    SinkInputType type;

    uint32_t index, clientIndex, sinkIndex;
    virtual void executeVolumeUpdate();
    virtual void onMuteToggleButton();
    virtual void onKill();
    virtual void prepareMenu();

    MainWindow *mainWindow;
    Gtk::Menu submenu;
    Gtk::MenuItem titleMenuItem, killMenuItem;

    struct SinkMenuItem {
        SinkMenuItem(SinkInputWidget *w, const char *label, uint32_t i, bool active) :
            widget(w),
            menuItem(label),
            index(i) {
            menuItem.set_active(active);
            menuItem.set_draw_as_radio(true);
            menuItem.signal_toggled().connect(sigc::mem_fun(*this, &SinkMenuItem::onToggle));
        }

        SinkInputWidget *widget;
        Gtk::CheckMenuItem menuItem;
        uint32_t index;
        void onToggle();
    };

    std::map<uint32_t, SinkMenuItem*> sinkMenuItems;

    void clearMenu();
    void buildMenu();
};

class SourceOutputWidget : public MinimalStreamWidget {
public:
    SourceOutputWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static SourceOutputWidget* create();
    virtual ~SourceOutputWidget();

    SourceOutputType type;

    uint32_t index, clientIndex, sourceIndex;
    virtual void onKill();

    MainWindow *mainWindow;
    Gtk::Menu submenu;
    Gtk::MenuItem titleMenuItem, killMenuItem;

    struct SourceMenuItem {
        SourceMenuItem(SourceOutputWidget *w, const char *label, uint32_t i, bool active) :
            widget(w),
            menuItem(label),
            index(i) {
            menuItem.set_active(active);
            menuItem.set_draw_as_radio(true);
            menuItem.signal_toggled().connect(sigc::mem_fun(*this, &SourceMenuItem::onToggle));
        }

        SourceOutputWidget *widget;
        Gtk::CheckMenuItem menuItem;
        uint32_t index;
        void onToggle();
    };

    std::map<uint32_t, SourceMenuItem*> sourceMenuItems;

    void clearMenu();
    void buildMenu();
    virtual void prepareMenu();
};

class MainWindow : public Gtk::Window {
public:
    MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static MainWindow* create();
    virtual ~MainWindow();

    void updateSink(const pa_sink_info &info);
    void updateSource(const pa_source_info &info);
    void updateSinkInput(const pa_sink_input_info &info);
    void updateSourceOutput(const pa_source_output_info &info);
    void updateClient(const pa_client_info &info);
    void updateServer(const pa_server_info &info);

    void removeSink(uint32_t index);
    void removeSource(uint32_t index);
    void removeSinkInput(uint32_t index);
    void removeSourceOutput(uint32_t index);
    void removeClient(uint32_t index);

    Gtk::VBox *streamsVBox, *recsVBox, *sinksVBox, *sourcesVBox;
    Gtk::Label *noStreamsLabel, *noRecsLabel, *noSinksLabel, *noSourcesLabel;
    Gtk::ComboBox *sinkInputTypeComboBox, *sourceOutputTypeComboBox, *sinkTypeComboBox, *sourceTypeComboBox;

    std::map<uint32_t, SinkWidget*> sinkWidgets;
    std::map<uint32_t, SourceWidget*> sourceWidgets;
    std::map<uint32_t, SinkInputWidget*> sinkInputWidgets;
    std::map<uint32_t, SourceOutputWidget*> sourceOutputWidgets;
    std::map<uint32_t, char*> clientNames;

    SinkInputType showSinkInputType;
    SinkType showSinkType;
    SourceOutputType showSourceOutputType;
    SourceType showSourceType;

    virtual void onSinkInputTypeComboBoxChanged();
    virtual void onSourceOutputTypeComboBoxChanged();
    virtual void onSinkTypeComboBoxChanged();
    virtual void onSourceTypeComboBoxChanged();

    void updateDeviceVisibility();

    Glib::ustring defaultSinkName, defaultSourceName;

protected:
    virtual void on_realize();
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
    Gtk::EventBox(cobject),
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
    double v;
    char txt[64];

    v = ((gdouble) volume * 100) / PA_VOLUME_NORM;

    if (can_decibel && show_decibel) {
        double dB = pa_sw_volume_to_dB(volume);

        if (dB > PA_DECIBEL_MININFTY) {
            snprintf(txt, sizeof(txt), "%0.2f dB", dB);
            volumeLabel->set_text(txt);
        } else
            volumeLabel->set_markup("-&#8734;dB");
    } else {
        snprintf(txt, sizeof(txt), "%0.0f%%", v);
        volumeLabel->set_text(txt);
    }

    volumeScaleEnabled = false;
    volumeScale->set_value(v > 100 ? 100 : v);
    volumeScaleEnabled = true;
}

void ChannelWidget::onVolumeScaleValueChanged() {

    if (!volumeScaleEnabled)
        return;

    if (streamWidget->updating)
        return;

    pa_volume_t volume = (pa_volume_t) ((volumeScale->get_value() * PA_VOLUME_NORM) / 100);
    streamWidget->updateChannelVolume(channel, volume);
}

void ChannelWidget::set_sensitive(bool enabled) {
    Gtk::EventBox::set_sensitive(enabled);

    channelLabel->set_sensitive(enabled);
    volumeLabel->set_sensitive(enabled);
    volumeScale->set_sensitive(enabled);
}

/*** MinimalStreamWidget ***/
MinimalStreamWidget::MinimalStreamWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    Gtk::VBox(cobject),
    updating(false) {

    x->get_widget("nameLabel", nameLabel);
    x->get_widget("boldNameLabel", boldNameLabel);
    x->get_widget("streamToggle", streamToggleButton);

    streamToggleButton->set_active(false);
    streamToggleButton->signal_clicked().connect(sigc::mem_fun(*this, &MinimalStreamWidget::onStreamToggleButton));
    menu.signal_deactivate().connect(sigc::mem_fun(*this, &MinimalStreamWidget::onMenuDeactivated));
}

void MinimalStreamWidget::prepareMenu(void) {
}

void MinimalStreamWidget::onMenuDeactivated(void) {
  streamToggleButton->set_active(false);
}

void MinimalStreamWidget::popupMenuPosition(int& x, int& y, bool& push_in G_GNUC_UNUSED) {
  Gtk::Requisition  r;

  streamToggleButton->get_window()->get_origin(x, y);
  r = menu.size_request();

  /* Align the right side of the menu with the right side of the togglebutton */
  x += streamToggleButton->get_allocation().get_x();
  x += streamToggleButton->get_allocation().get_width();
  x -= r.width;

  /* Align the top of the menu with the buttom of the togglebutton */
  y += streamToggleButton->get_allocation().get_y();
  y += streamToggleButton->get_allocation().get_height();
}

void MinimalStreamWidget::onStreamToggleButton(void) {
  if (streamToggleButton->get_active()) {
    prepareMenu();
    menu.popup(sigc::mem_fun(*this, &MinimalStreamWidget::popupMenuPosition), 0, gtk_get_current_event_time());
  }
}

bool MinimalStreamWidget::on_button_press_event (GdkEventButton* event) {
   if (Gtk::VBox::on_button_press_event(event))
        return TRUE;

    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        prepareMenu();
        menu.popup(0, event->time);
        return TRUE;
    }

    return FALSE;
}

/*** StreamWidget ***/

StreamWidget::StreamWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    MinimalStreamWidget(cobject, x) {

    x->get_widget("channelsVBox", channelsVBox);
    x->get_widget("lockToggleButton", lockToggleButton);
    x->get_widget("muteToggleButton", muteToggleButton);

    muteToggleButton->signal_clicked().connect(sigc::mem_fun(*this, &StreamWidget::onMuteToggleButton));

    for (int i = 0; i < PA_CHANNELS_MAX; i++)
        channelWidgets[i] = NULL;
}

void StreamWidget::setChannelMap(const pa_channel_map &m, bool can_decibel) {
    channelMap = m;

    for (int i = 0; i < m.channels; i++) {
        ChannelWidget *cw = channelWidgets[i] = ChannelWidget::create();
        cw->channel = i;
        cw->can_decibel = can_decibel;
        cw->streamWidget = this;
        char text[64];
        snprintf(text, sizeof(text), "<b>%s</b>", pa_channel_position_to_pretty_string(m.map[i]));
        cw->channelLabel->set_markup(text);
        channelsVBox->pack_start(*cw, false, false, 0);
    }

    lockToggleButton->set_sensitive(m.channels > 1);
}

void StreamWidget::setVolume(const pa_cvolume &v, bool force = false) {
    g_assert(v.channels == channelMap.channels);

    volume = v;

    if (timeoutConnection.empty() || force) { /* do not update the volume when a volume change is still in flux */
        for (int i = 0; i < volume.channels; i++)
            channelWidgets[i]->setVolume(volume.values[i]);
    }
}

void StreamWidget::updateChannelVolume(int channel, pa_volume_t v) {
    pa_cvolume n;
    g_assert(channel < volume.channels);

    n = volume;
    if (lockToggleButton->get_active()) {
        for (int i = 0; i < n.channels; i++)
            n.values[i] = v;
    } else
        n.values[channel] = v;

    setVolume(n, true);

    if (timeoutConnection.empty())
        timeoutConnection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &StreamWidget::timeoutEvent), 100);
}

void StreamWidget::onMuteToggleButton() {

    lockToggleButton->set_sensitive(!muteToggleButton->get_active());

    for (int i = 0; i < channelMap.channels; i++)
        channelWidgets[i]->set_sensitive(!muteToggleButton->get_active());
}

bool StreamWidget::timeoutEvent() {
    executeVolumeUpdate();
    return false;
}

void StreamWidget::executeVolumeUpdate() {
}

SinkWidget::SinkWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    StreamWidget(cobject, x),
    defaultMenuItem("_Default", true){

    add_events(Gdk::BUTTON_PRESS_MASK);

    defaultMenuItem.set_active(false);
    defaultMenuItem.signal_toggled().connect(sigc::mem_fun(*this, &SinkWidget::onDefaultToggle));
    menu.append(defaultMenuItem);
    menu.show_all();
}

SinkWidget* SinkWidget::create() {
    SinkWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    return w;
}

void SinkWidget::executeVolumeUpdate() {
    pa_operation* o;

    if (!(o = pa_context_set_sink_volume_by_index(context, index, &volume, NULL, NULL))) {
        show_error("pa_context_set_sink_volume_by_index() failed");
        return;
    }

    pa_operation_unref(o);
}

void SinkWidget::onMuteToggleButton() {
    StreamWidget::onMuteToggleButton();

    if (updating)
        return;

    pa_operation* o;
    if (!(o = pa_context_set_sink_mute_by_index(context, index, muteToggleButton->get_active(), NULL, NULL))) {
        show_error("pa_context_set_sink_mute_by_index() failed");
        return;
    }

    pa_operation_unref(o);
}

void SinkWidget::onDefaultToggle() {
    pa_operation* o;

    if (updating)
        return;

    if (!(o = pa_context_set_default_sink(context, name.c_str(), NULL, NULL))) {
        show_error("pa_context_set_default_sink() failed");
        return;
    }
    pa_operation_unref(o);
}

SourceWidget::SourceWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    StreamWidget(cobject, x),
    defaultMenuItem("_Default", true){

    add_events(Gdk::BUTTON_PRESS_MASK);

    defaultMenuItem.set_active(false);
    defaultMenuItem.signal_toggled().connect(sigc::mem_fun(*this, &SourceWidget::onDefaultToggle));
    menu.append(defaultMenuItem);
    menu.show_all();
}

SourceWidget* SourceWidget::create() {
    SourceWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    return w;
}

void SourceWidget::executeVolumeUpdate() {
    pa_operation* o;

    if (!(o = pa_context_set_source_volume_by_index(context, index, &volume, NULL, NULL))) {
        show_error("pa_context_set_source_volume_by_index() failed");
        return;
    }

    pa_operation_unref(o);
}

void SourceWidget::onMuteToggleButton() {
    StreamWidget::onMuteToggleButton();

    if (updating)
        return;

    pa_operation* o;
    if (!(o = pa_context_set_source_mute_by_index(context, index, muteToggleButton->get_active(), NULL, NULL))) {
        show_error("pa_context_set_source_mute_by_index() failed");
        return;
    }

    pa_operation_unref(o);
}

void SourceWidget::onDefaultToggle() {
    pa_operation* o;

    if (updating)
        return;

    if (!(o = pa_context_set_default_source(context, name.c_str(), NULL, NULL))) {
        show_error("pa_context_set_default_source() failed");
        return;
    }
    pa_operation_unref(o);
}

SinkInputWidget::SinkInputWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    StreamWidget(cobject, x),
    mainWindow(NULL),
    titleMenuItem("_Move Stream...", true),
    killMenuItem("_Terminate Stream", true) {

    add_events(Gdk::BUTTON_PRESS_MASK);

    menu.append(titleMenuItem);
    titleMenuItem.set_submenu(submenu);

    menu.append(killMenuItem);
    killMenuItem.signal_activate().connect(sigc::mem_fun(*this, &SinkInputWidget::onKill));
}

SinkInputWidget::~SinkInputWidget() {
    clearMenu();
}

SinkInputWidget* SinkInputWidget::create() {
    SinkInputWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    return w;
}

void SinkInputWidget::executeVolumeUpdate() {
    pa_operation* o;

    if (!(o = pa_context_set_sink_input_volume(context, index, &volume, NULL, NULL))) {
        show_error("pa_context_set_sink_input_volume() failed");
        return;
    }

    pa_operation_unref(o);
}

void SinkInputWidget::onMuteToggleButton() {
    StreamWidget::onMuteToggleButton();

    if (updating)
        return;

    pa_operation* o;
    if (!(o = pa_context_set_sink_input_mute(context, index, muteToggleButton->get_active(), NULL, NULL))) {
        show_error("pa_context_set_sink_input_mute() failed");
        return;
    }

    pa_operation_unref(o);
}

void SinkInputWidget::prepareMenu() {
  clearMenu();
  buildMenu();
}

void SinkInputWidget::clearMenu() {

    while (!sinkMenuItems.empty()) {
        std::map<uint32_t, SinkMenuItem*>::iterator i = sinkMenuItems.begin();
        delete i->second;
        sinkMenuItems.erase(i);
    }
}

void SinkInputWidget::buildMenu() {
    for (std::map<uint32_t, SinkWidget*>::iterator i = mainWindow->sinkWidgets.begin(); i != mainWindow->sinkWidgets.end(); ++i) {
        SinkMenuItem *m;
        sinkMenuItems[i->second->index] = m = new SinkMenuItem(this, i->second->description.c_str(), i->second->index, i->second->index == sinkIndex);
        submenu.append(m->menuItem);
    }

    menu.show_all();
}

void SinkInputWidget::onKill() {
    pa_operation* o;
    if (!(o = pa_context_kill_sink_input(context, index, NULL, NULL))) {
        show_error("pa_context_kill_sink_input() failed");
        return;
    }

    pa_operation_unref(o);
}

void SinkInputWidget::SinkMenuItem::onToggle() {

    if (widget->updating)
        return;

    if (!menuItem.get_active())
        return;

    pa_operation* o;
    if (!(o = pa_context_move_sink_input_by_index(context, widget->index, index, NULL, NULL))) {
        show_error("pa_context_move_sink_input_by_index() failed");
        return;
    }

    pa_operation_unref(o);
}

SourceOutputWidget::SourceOutputWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    MinimalStreamWidget(cobject, x),
    mainWindow(NULL),
    titleMenuItem("_Move Stream...", true),
    killMenuItem("_Terminate Stream", true) {

    add_events(Gdk::BUTTON_PRESS_MASK);

    menu.append(titleMenuItem);
    titleMenuItem.set_submenu(submenu);

    menu.append(killMenuItem);
    killMenuItem.signal_activate().connect(sigc::mem_fun(*this, &SourceOutputWidget::onKill));
}

SourceOutputWidget::~SourceOutputWidget() {
    clearMenu();
}

SourceOutputWidget* SourceOutputWidget::create() {
    SourceOutputWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "minimalStreamWidget");
    x->get_widget_derived("minimalStreamWidget", w);
    return w;
}

void SourceOutputWidget::onKill() {
    pa_operation* o;
    if (!(o = pa_context_kill_source_output(context, index, NULL, NULL))) {
        show_error("pa_context_kill_source_output() failed");
        return;
    }

    pa_operation_unref(o);
}

void SourceOutputWidget::clearMenu() {

    while (!sourceMenuItems.empty()) {
        std::map<uint32_t, SourceMenuItem*>::iterator i = sourceMenuItems.begin();
        delete i->second;
        sourceMenuItems.erase(i);
    }
}

void SourceOutputWidget::buildMenu() {
    for (std::map<uint32_t, SourceWidget*>::iterator i = mainWindow->sourceWidgets.begin(); i != mainWindow->sourceWidgets.end(); ++i) {
        SourceMenuItem *m;
        sourceMenuItems[i->second->index] = m = new SourceMenuItem(this, i->second->description.c_str(), i->second->index, i->second->index == sourceIndex);
        submenu.append(m->menuItem);
    }

    menu.show_all();
}

void SourceOutputWidget::prepareMenu(void) {
  clearMenu();
  buildMenu();
}

void SourceOutputWidget::SourceMenuItem::onToggle() {

    if (widget->updating)
        return;

    if (!menuItem.get_active())
        return;

    pa_operation* o;
    if (!(o = pa_context_move_source_output_by_index(context, widget->index, index, NULL, NULL))) {
        show_error("pa_context_move_source_output_by_index() failed");
        return;
    }

    pa_operation_unref(o);
}

/*** MainWindow ***/

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    Gtk::Window(cobject),
    showSinkInputType(SINK_INPUT_CLIENT),
    showSinkType(SINK_ALL),
    showSourceType(SOURCE_NO_MONITOR) {

    x->get_widget("streamsVBox", streamsVBox);
    x->get_widget("recsVBox", recsVBox);
    x->get_widget("sinksVBox", sinksVBox);
    x->get_widget("sourcesVBox", sourcesVBox);
    x->get_widget("noStreamsLabel", noStreamsLabel);
    x->get_widget("noRecsLabel", noRecsLabel);
    x->get_widget("noSinksLabel", noSinksLabel);
    x->get_widget("noSourcesLabel", noSourcesLabel);
    x->get_widget("sinkInputTypeComboBox", sinkInputTypeComboBox);
    x->get_widget("sourceOutputTypeComboBox", sourceOutputTypeComboBox);
    x->get_widget("sinkTypeComboBox", sinkTypeComboBox);
    x->get_widget("sourceTypeComboBox", sourceTypeComboBox);

    sourcesVBox->set_reallocate_redraws(true);
    streamsVBox->set_reallocate_redraws(true);
    sinksVBox->set_reallocate_redraws(true);

    sinkInputTypeComboBox->set_active((int) showSinkInputType);
    sourceOutputTypeComboBox->set_active((int) showSourceOutputType);
    sinkTypeComboBox->set_active((int) showSinkType);
    sourceTypeComboBox->set_active((int) showSourceType);

    sinkInputTypeComboBox->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::onSinkInputTypeComboBoxChanged));
    sourceOutputTypeComboBox->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::onSourceOutputTypeComboBoxChanged));
    sinkTypeComboBox->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::onSinkTypeComboBoxChanged));
    sourceTypeComboBox->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::onSourceTypeComboBoxChanged));
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

void MainWindow::updateSink(const pa_sink_info &info) {
    SinkWidget *w;
    bool is_new = false;

    if (sinkWidgets.count(info.index))
        w = sinkWidgets[info.index];
    else {
        sinkWidgets[info.index] = w = SinkWidget::create();
        w->setChannelMap(info.channel_map, !!(info.flags & PA_SINK_DECIBEL_VOLUME));
        sinksVBox->pack_start(*w, false, false, 0);
        w->index = info.index;
        is_new = true;
    }

    w->updating = true;

    w->name = info.name;
    w->description = info.description;
    w->type = info.flags & PA_SINK_HARDWARE ? SINK_HARDWARE : SINK_VIRTUAL;

    w->boldNameLabel->set_text("");
    gchar *txt;
    w->nameLabel->set_markup(txt = g_markup_printf_escaped("%s", info.description));
    g_free(txt);

    w->setVolume(info.volume);
    w->muteToggleButton->set_active(info.mute);

    w->defaultMenuItem.set_active(w->name == defaultSinkName);

    if (is_new)
        updateDeviceVisibility();

    w->updating = false;
}

void MainWindow::updateSource(const pa_source_info &info) {
    SourceWidget *w;
    bool is_new = false;

    if (sourceWidgets.count(info.index))
        w = sourceWidgets[info.index];
    else {
        sourceWidgets[info.index] = w = SourceWidget::create();
        w->setChannelMap(info.channel_map, !!(info.flags & PA_SOURCE_DECIBEL_VOLUME));
        sourcesVBox->pack_start(*w, false, false, 0);
        w->index = info.index;
        is_new = true;
    }

    w->updating = true;

    w->name = info.name;
    w->description = info.description;
    w->type = info.monitor_of_sink != PA_INVALID_INDEX ? SOURCE_MONITOR : (info.flags & PA_SOURCE_HARDWARE ? SOURCE_HARDWARE : SOURCE_VIRTUAL);

    w->boldNameLabel->set_text("");
    gchar *txt;
    w->nameLabel->set_markup(txt = g_markup_printf_escaped("%s", info.description));
    g_free(txt);

    w->setVolume(info.volume);
    w->muteToggleButton->set_active(info.mute);

    w->defaultMenuItem.set_active(w->name == defaultSourceName);

    if (is_new)
        updateDeviceVisibility();

    w->updating = false;
}

void MainWindow::updateSinkInput(const pa_sink_input_info &info) {
    SinkInputWidget *w;
    bool is_new = false;

    if (sinkInputWidgets.count(info.index))
        w = sinkInputWidgets[info.index];
    else {
        sinkInputWidgets[info.index] = w = SinkInputWidget::create();
        w->setChannelMap(info.channel_map, true);
        streamsVBox->pack_start(*w, false, false, 0);
        w->index = info.index;
        w->clientIndex = info.client;
        w->mainWindow = this;
        is_new = true;
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

    w->setVolume(info.volume);
    w->muteToggleButton->set_active(info.mute);

    if (is_new)
        updateDeviceVisibility();

    w->updating = false;
}

void MainWindow::updateSourceOutput(const pa_source_output_info &info) {
    SourceOutputWidget *w;
    bool is_new = false;

    if (sourceOutputWidgets.count(info.index))
        w = sourceOutputWidgets[info.index];
    else {
        sourceOutputWidgets[info.index] = w = SourceOutputWidget::create();
        recsVBox->pack_start(*w, false, false, 0);
        w->index = info.index;
        w->clientIndex = info.client;
        w->mainWindow = this;
        is_new = true;
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

    if (is_new)
        updateDeviceVisibility();

    w->updating = false;
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

void MainWindow::updateDeviceVisibility() {
    streamsVBox->hide_all();
    recsVBox->hide_all();
    sourcesVBox->hide_all();
    sinksVBox->hide_all();

    bool is_empty = true;

    for (std::map<uint32_t, SinkInputWidget*>::iterator i = sinkInputWidgets.begin(); i != sinkInputWidgets.end(); ++i) {
        SinkInputWidget* w = i->second;

        if (showSinkInputType == SINK_INPUT_ALL || w->type == showSinkInputType) {
            w->show_all();
            is_empty = false;
        }
    }

    if (is_empty)
        noStreamsLabel->show();

    for (std::map<uint32_t, SourceOutputWidget*>::iterator i = sourceOutputWidgets.begin(); i != sourceOutputWidgets.end(); ++i) {
        SourceOutputWidget* w = i->second;

        if (showSourceOutputType == SOURCE_OUTPUT_ALL || w->type == showSourceOutputType) {
            w->show_all();
            is_empty = false;
        }
    }

    if (is_empty)
        noRecsLabel->show();

    is_empty = true;

    for (std::map<uint32_t, SinkWidget*>::iterator i = sinkWidgets.begin(); i != sinkWidgets.end(); ++i) {
        SinkWidget* w = i->second;

        if (showSinkType == SINK_ALL || w->type == showSinkType) {
            w->show_all();
            is_empty = false;
        }
    }

    if (is_empty)
        noSinksLabel->show();

    is_empty = true;

    for (std::map<uint32_t, SourceWidget*>::iterator i = sourceWidgets.begin(); i != sourceWidgets.end(); ++i) {
        SourceWidget* w = i->second;

        if (showSourceType == SOURCE_ALL ||
            w->type == showSourceType ||
            (showSourceType == SOURCE_NO_MONITOR && w->type != SOURCE_MONITOR)) {
            w->show_all();
            is_empty = false;
        }
    }

    if (is_empty)
        noSourcesLabel->show();

    sourcesVBox->show();
    recsVBox->show();
    sinksVBox->show();
    streamsVBox->show();
}

void MainWindow::removeSink(uint32_t index) {
    if (!sinkWidgets.count(index))
        return;

    delete sinkWidgets[index];
    sinkWidgets.erase(index);
    updateDeviceVisibility();
}

void MainWindow::removeSource(uint32_t index) {
    if (!sourceWidgets.count(index))
        return;

    delete sourceWidgets[index];
    sourceWidgets.erase(index);
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

static void dec_outstanding(MainWindow *w) {
    if (n_outstanding <= 0)
        return;

    if (--n_outstanding <= 0)
        w->get_window()->set_cursor();
}

void sink_cb(pa_context *, const pa_sink_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol) {
        dec_outstanding(w);
        return;
    }

    if (!i) {
        show_error("Sink callback failure");
        return;
    }

    w->updateSink(*i);
}

void source_cb(pa_context *, const pa_source_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol) {
        dec_outstanding(w);
        return;
    }

    if (!i) {
        show_error("Source callback failure");
        return;
    }

    w->updateSource(*i);
}

void sink_input_cb(pa_context *, const pa_sink_input_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol) {
        dec_outstanding(w);
        return;
    }

    if (!i) {
        show_error("Sink input callback failure");
        return;
    }

    w->updateSinkInput(*i);
}

void source_output_cb(pa_context *, const pa_source_output_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol) {
        dec_outstanding(w);
        return;
    }

    if (!i) {
        show_error("Source output callback failure");
        return;
    }

    w->updateSourceOutput(*i);
}

void client_cb(pa_context *, const pa_client_info *i, int eol, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (eol) {
        dec_outstanding(w);
        return;
    }

    if (!i) {
        show_error("Client callback failure");
        return;
    }

    w->updateClient(*i);
}

void server_info_cb(pa_context *, const pa_server_info *i, void *userdata) {
    MainWindow *w = static_cast<MainWindow*>(userdata);

    if (!i) {
        show_error("Server info callback failure");
        return;
    }

    w->updateServer(*i);
    dec_outstanding(w);
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

        case PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                w->removeSourceOutput(index);
            else {
                pa_operation *o;
                if (!(o = pa_context_get_source_output_info(c, index, source_output_cb, w))) {
                    show_error("pa_context_get_sink_input_info() failed");
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
                    show_error("pa_context_get_client_info() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SERVER: {
            pa_operation *o;
            if (!(o = pa_context_get_server_info(c, server_info_cb, w))) {
                show_error("pa_context_get_server_info() failed");
                return;
            }
            pa_operation_unref(o);
        }
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

            if (!(o = pa_context_subscribe(c, (pa_subscription_mask_t)
                                           (PA_SUBSCRIPTION_MASK_SINK|
                                            PA_SUBSCRIPTION_MASK_SOURCE|
                                            PA_SUBSCRIPTION_MASK_SINK_INPUT|
                                            PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT|
                                            PA_SUBSCRIPTION_MASK_CLIENT|
                                            PA_SUBSCRIPTION_MASK_SERVER), NULL, NULL))) {
                show_error("pa_context_subscribe() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_server_info(c, server_info_cb, w))) {
                show_error("pa_context_get_server_info() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_client_info_list(c, client_cb, w))) {
                show_error("pa_context_client_info_list() failed");
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

            if (!(o = pa_context_get_source_output_info_list(c, source_output_cb, w))) {
                show_error("pa_context_get_source_output_info_list() failed");
                return;
            }
            pa_operation_unref(o);

            n_outstanding = 6;

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
    context = pa_context_new(api, "PulseAudio Volume Control");
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
