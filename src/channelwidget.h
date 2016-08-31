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

#ifndef channelwidget_h
#define channelwidget_h

#include "pavucontrol.h"
#include "ui_channelwidget.h"

class MinimalStreamWidget;

class ChannelWidget : public QWidget, public Ui::ChannelWidget {
    Q_OBJECT
public:
    ChannelWidget(QWidget* parent=nullptr);

    void setVolume(pa_volume_t volume);

    int channel;
    MinimalStreamWidget *minimalStreamWidget;

protected Q_SLOTS:
    void onVolumeScaleValueChanged(int value);

public:
    bool can_decibel;
    bool volumeScaleEnabled;
    bool last;

    //virtual void set_sensitive(bool enabled);
    virtual void setBaseVolume(pa_volume_t);
};


#endif
