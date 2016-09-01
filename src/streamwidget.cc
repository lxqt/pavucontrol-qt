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
#include "mainwindow.h"
#include "channelwidget.h"


/*** StreamWidget ***/
StreamWidget::StreamWidget(MainWindow *parent) :
    MinimalStreamWidget(parent),
    mpMainWindow(parent) {

    setupUi(this);
    initPeakProgressBar(channelsVBox);

    timeout.setSingleShot(true);
    timeout.setInterval(100);
    connect(&timeout, &QTimer::timeout, this, &StreamWidget::timeoutEvent);

//    this->signal_button_press_event().connect(sigc::mem_fun(*this, &StreamWidget::onContextTriggerEvent));
    connect(muteToggleButton, &QToolButton::toggled, this, &StreamWidget::onMuteToggleButton);
    connect(lockToggleButton, &QToolButton::toggled, this, &StreamWidget::onLockToggleButton);
//    connect(deviceButton->signal_clicked().connect(sigc::mem_fun(*this, &StreamWidget::onDeviceChangePopup));

#if 0

    terminate.set_label(tr("Terminate").toUtf8().constData());
    terminate.signal_activate().connect(sigc::mem_fun(*this, &StreamWidget::onKill));
    contextMenu.append(terminate);
    contextMenu.show_all();
#endif
    for (unsigned i = 0; i < PA_CHANNELS_MAX; i++)
        channelWidgets[i] = NULL;
}


#if 0
bool StreamWidget::onContextTriggerEvent(GdkEventButton* event) {
    if (GDK_BUTTON_PRESS == event->type && 3 == event->button) {
        contextMenu.popup(event->button, event->time);
        return true;
    }
    return false;
}
#endif

void StreamWidget::setChannelMap(const pa_channel_map &m, bool can_decibel) {
    channelMap = m;

    for (int i = 0; i < m.channels; i++) {
        ChannelWidget *cw = channelWidgets[i] = new ChannelWidget(this);
        cw->channel = i;
        cw->can_decibel = can_decibel;
        cw->minimalStreamWidget = this;
        char text[64];
        snprintf(text, sizeof(text), "<b>%s</b>", pa_channel_position_to_pretty_string(m.map[i]));
        cw->channelLabel->setText(QString::fromUtf8(text));
        channelsVBox->addWidget(cw);
    }
    channelWidgets[m.channels-1]->last = true;
    channelWidgets[m.channels-1]->setBaseVolume(PA_VOLUME_NORM);

    lockToggleButton->setEnabled(m.channels > 1);
    hideLockedChannels(lockToggleButton->isChecked());
}

void StreamWidget::setVolume(const pa_cvolume &v, bool force) {
    g_assert(v.channels == channelMap.channels);

    volume = v;

    if (!timeout.isActive() || force) { /* do not update the volume when a volume change is still in flux */
        for (int i = 0; i < volume.channels; i++)
            channelWidgets[i]->setVolume(volume.values[i]);
    }
}

void StreamWidget::updateChannelVolume(int channel, pa_volume_t v) {
    pa_cvolume n;
    g_assert(channel < volume.channels);

    n = volume;
    if (lockToggleButton->isChecked()) {
        for (int i = 0; i < n.channels; i++)
            n.values[i] = v;
    } else
        n.values[channel] = v;

    setVolume(n, true);

    if(!timeout.isActive()) {
        timeout.start();
    }
}

void StreamWidget::hideLockedChannels(bool hide) {
    for (int i = 0; i < channelMap.channels - 1; i++)
        channelWidgets[i]->setVisible(!hide);

    channelWidgets[channelMap.channels - 1]->channelLabel->setVisible(!hide);
}

void StreamWidget::onMuteToggleButton() {

    lockToggleButton->setEnabled(!muteToggleButton->isChecked());

    for (int i = 0; i < channelMap.channels; i++)
        channelWidgets[i]->setEnabled(!muteToggleButton->isChecked());
}

void StreamWidget::onLockToggleButton() {
    hideLockedChannels(lockToggleButton->isChecked());
}

bool StreamWidget::timeoutEvent() {
    executeVolumeUpdate();
    return false;
}

void StreamWidget::executeVolumeUpdate() {
}

void StreamWidget::onDeviceChangePopup() {
}

void StreamWidget::onKill() {
}
