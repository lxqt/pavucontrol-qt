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

#include "streamwidget.h"
#include "channelwidget.h"

/*** StreamWidget ***/

StreamWidget::StreamWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    MinimalStreamWidget(cobject, x)  {

    x->get_widget("lockToggleButton", lockToggleButton);
    x->get_widget("muteToggleButton", muteToggleButton);

    muteToggleButton->signal_clicked().connect(sigc::mem_fun(*this, &StreamWidget::onMuteToggleButton));

    for (unsigned i = 0; i < PA_CHANNELS_MAX; i++)
        channelWidgets[i] = NULL;
}

void StreamWidget::setChannelMap(const pa_channel_map &m, bool can_decibel) {
    channelMap = m;

    for (int i = 0; i < m.channels; i++) {
        ChannelWidget *cw = channelWidgets[i] = ChannelWidget::create();
        cw->beepDevice = beepDevice;
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

void StreamWidget::setVolume(const pa_cvolume &v, bool force) {
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


void StreamWidget::setBaseVolume(pa_volume_t v) {

    if (channelMap.channels > 0)
        channelWidgets[channelMap.channels-1]->setBaseVolume(v);
}

void StreamWidget::setSteps(unsigned n) {

    for (int i = 0; i < channelMap.channels; i++)
        channelWidgets[channelMap.channels-1]->setSteps(n);
}
