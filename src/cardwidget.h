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

#ifndef cardwidget_h
#define cardwidget_h

#include "pavucontrol.h"
#include "ui_cardwidget.h"
#include <QWidget>

class PortInfo {
public:
      Glib::ustring name;
      Glib::ustring description;
      uint32_t priority;
      int available;
      int direction;
      int64_t latency_offset;
      std::vector<Glib::ustring> profiles;
};

class CardWidget : public QWidget, public Ui::CardWidget {
    Q_OBJECT
public:
    CardWidget(QWidget *parent = nullptr);

    Glib::ustring name;
    uint32_t index;
    bool updating;

    std::vector< std::pair<Glib::ustring,Glib::ustring> > profiles;
    std::map<Glib::ustring, PortInfo> ports;
    Glib::ustring activeProfile;
    bool hasSinks;
    bool hasSources;

    void prepareMenu();

protected:
  virtual void onProfileChange(int active);

};

#endif
