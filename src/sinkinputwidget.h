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

#ifndef sinkinputwidget_h
#define sinkinputwidget_h

#include "pavucontrol.h"

#include "streamwidget.h"

class MainWindow;

class SinkInputWidget : public StreamWidget {
public:
    SinkInputWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static SinkInputWidget* create(MainWindow* mainWindow);
    ~SinkInputWidget(void);

    void init(MainWindow* mainWindow);

    SinkInputType type;

    uint32_t index, clientIndex;
    void setSinkIndex(uint32_t idx);
    uint32_t sinkIndex();
    virtual void executeVolumeUpdate();
    virtual bool onDeviceChangePopup(GdkEventButton*);
    virtual bool onWidgetButtonEvent(GdkEventButton*);
    virtual void onMuteToggleButton();
    virtual void onKill();

private:
    MainWindow *mpMainWindow;
    uint32_t mSinkIndex;

    Gtk::Menu terminateMenu;
    Gtk::MenuItem terminate;

    void clearMenu();
    void buildMenu();

    Gtk::Menu menu;

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
};

#endif
