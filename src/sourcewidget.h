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

#ifndef sourcewidget_h
#define sourcewidget_h

#include "pavucontrol.h"

#include "streamwidget.h"

class SourceWidget : public StreamWidget {
public:
    SourceWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x);
    static SourceWidget* create();

    SourceType type;
    Glib::ustring name;
    Glib::ustring description;
    uint32_t index, card_index;
    bool can_decibel;

    Gtk::CheckMenuItem defaultMenuItem;

    virtual void onMuteToggleButton();
    virtual void executeVolumeUpdate();
    virtual void onDefaultToggle();
};

#endif
