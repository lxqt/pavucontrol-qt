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

#ifndef mainwindow_h
#define mainwindow_h

#include "pavucontrol.h"
#include <pulse/ext-stream-restore.h>


class CardWidget;
class SinkWidget;
class SourceWidget;
class SinkInputWidget;
class SourceOutputWidget;
class RoleWidget;

class MainWindow : public Gtk::Window {
public:
    MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static MainWindow* create();
    virtual ~MainWindow();

    void updateCard(const pa_card_info &info);
    void updateSink(const pa_sink_info &info);
    void updateSource(const pa_source_info &info);
    void updateSinkInput(const pa_sink_input_info &info);
    void updateSourceOutput(const pa_source_output_info &info);
    void updateClient(const pa_client_info &info);
    void updateServer(const pa_server_info &info);
    void updateVolumeMeter(uint32_t source_index, uint32_t sink_input_index, double v);
    void updateRole(const pa_ext_stream_restore_info &info);

    void removeCard(uint32_t index);
    void removeSink(uint32_t index);
    void removeSource(uint32_t index);
    void removeSinkInput(uint32_t index);
    void removeSourceOutput(uint32_t index);
    void removeClient(uint32_t index);

    Gtk::Notebook *notebook;
    Gtk::VBox *streamsVBox, *recsVBox, *sinksVBox, *sourcesVBox, *cardsVBox;
    Gtk::Label *noStreamsLabel, *noRecsLabel, *noSinksLabel, *noSourcesLabel, *noCardsLabel;
    Gtk::ComboBox *sinkInputTypeComboBox, *sourceOutputTypeComboBox, *sinkTypeComboBox, *sourceTypeComboBox;

    std::map<uint32_t, CardWidget*> cardWidgets;
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
    void reallyUpdateDeviceVisibility();
    void createMonitorStreamForSource(uint32_t source_idx);
    void createMonitorStreamForSinkInput(uint32_t sink_input_idx, uint32_t sink_idx);

    void setIconFromProplist(Gtk::Image *icon, pa_proplist *l, const char *name);

    RoleWidget *eventRoleWidget;

    bool createEventRoleWidget();
    void deleteEventRoleWidget();

    Glib::ustring defaultSinkName, defaultSourceName;

protected:
    virtual void on_realize();
};


#endif
