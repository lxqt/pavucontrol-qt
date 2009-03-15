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

#ifndef minimalstreamwidget_h
#define minimalstreamwidget_h

#include "pavucontrol.h"

class MinimalStreamWidget : public Gtk::VBox {
public:
    MinimalStreamWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);

    Gtk::VBox *channelsVBox;
    Gtk::Label *nameLabel, *boldNameLabel;
    Gtk::ToggleButton *streamToggleButton;
    Gtk::Menu menu;
    Gtk::Image *iconImage;
    Gtk::ProgressBar peakProgressBar;
    double lastPeak;

    bool updating;

    void onStreamToggleButton();
    void onMenuDeactivated();
    void popupMenuPosition(int& x, int& y, bool& push_in);

    virtual void prepareMenu(void);

    bool volumeMeterEnabled;
    void enableVolumeMeter();
    void updatePeak(double v);

    Glib::ustring beepDevice;

protected:
    virtual bool on_button_press_event(GdkEventButton* event);
};

#endif
