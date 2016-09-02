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

#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include "channel.h"
#include "minimalstreamwidget.h"

/*** ChannelWidget ***/

Channel::Channel(QGridLayout* parent) :
    QObject(parent),
    can_decibel(false),
    volumeScaleEnabled(true),
    last(false)
{
    channelLabel = new QLabel(nullptr);
    volumeScale = new QSlider(Qt::Horizontal, nullptr);
    volumeLabel = new QLabel(nullptr);

    const int row = parent->rowCount();
    parent->addWidget(channelLabel, row, 0);
    parent->addWidget(volumeScale, row, 1);
    parent->addWidget(volumeLabel, row, 2);

    volumeScale->setMinimum(PA_VOLUME_MUTED);
    volumeScale->setMaximum(PA_VOLUME_UI_MAX);
    volumeScale->setValue(PA_VOLUME_NORM);
    volumeScale->setSingleStep(qRound((double)PA_VOLUME_NORM)/100.0);
    volumeScale->setPageStep(qRound((double)PA_VOLUME_NORM)/20.0);
    setBaseVolume(PA_VOLUME_NORM);

    connect(volumeScale, &QSlider::valueChanged, this, &Channel::onVolumeScaleValueChanged);
}

void Channel::setVolume(pa_volume_t volume) {
    double v;
    char txt[64];

    v = ((gdouble) volume * 100) / PA_VOLUME_NORM;
    if (can_decibel) {
        double dB = pa_sw_volume_to_dB(volume);
        if (dB > PA_DECIBEL_MININFTY)
            snprintf(txt, sizeof(txt), "<small>%0.0f%% (%0.2fdB)</small>", v, dB);
        else
            snprintf(txt, sizeof(txt), "<small>%0.0f%% (-&#8734;dB)</small>", v);
    }
    else
        snprintf(txt, sizeof(txt), "%0.0f%%", v);
    volumeLabel->setText(QString::fromUtf8(txt));

    volumeScaleEnabled = false;
    volumeScale->setValue(volume > PA_VOLUME_UI_MAX ? PA_VOLUME_UI_MAX : volume);
    volumeScaleEnabled = true;
}

void Channel::setVisible(bool visible)
{
    channelLabel->setVisible(visible);
    volumeScale->setVisible(visible);
    volumeLabel->setVisible(visible);
}

void Channel::setEnabled(bool enabled)
{
    channelLabel->setEnabled(enabled);
    volumeScale->setEnabled(enabled);
    volumeLabel->setEnabled(enabled);
}

void Channel::onVolumeScaleValueChanged(int value) {

    if (!volumeScaleEnabled)
        return;

    if (minimalStreamWidget->updating)
        return;

    pa_volume_t volume = (pa_volume_t) volumeScale->value();
    minimalStreamWidget->updateChannelVolume(channel, volume);
}

/*
void Channel::set_sensitive(bool enabled) {
    setEnabled(enabled);

    channelLabel->setEnabled(enabled);
    volumeLabel->setEnabled(enabled);
    volumeScale->setEnabled(enabled);
}
*/

void Channel::setBaseVolume(pa_volume_t v) {
#if 0  // FIXME: Qt does not support this functionality
    gtk_scale_clear_marks(GTK_SCALE(volumeScale->gobj()));

    gtk_scale_add_mark(GTK_SCALE(volumeScale->gobj()), (double)PA_VOLUME_MUTED, (GtkPositionType) GTK_POS_BOTTOM,
                       last ? (can_decibel ? tr("<small>Silence</small>").toUtf8().constData() : tr("<small>Min</small>").toUtf8().constData()) : NULL);
    gtk_scale_add_mark(GTK_SCALE(volumeScale->gobj()), (double)PA_VOLUME_NORM, (GtkPositionType) GTK_POS_BOTTOM,
                       last ? tr("<small>100% (0dB)</small>").toUtf8().constData() : NULL);
    if (v > PA_VOLUME_MUTED && v < PA_VOLUME_NORM) {
        gtk_scale_add_mark(GTK_SCALE(volumeScale->gobj()), (double)v, (GtkPositionType) GTK_POS_BOTTOM,
                           last ? tr("<small><i>Base</i></small>").toUtf8().constData() : NULL);
    }
#endif

}
