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
  along with pavucontrol. If not, see <https://www.gnu.org/licenses/>.
***/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "minimalstreamwidget.h"
#include <QGridLayout>
#include <QProgressBar>
#include <QPropertyAnimation>
#include <QDebug>

/*** MinimalStreamWidget ***/
MinimalStreamWidget::MinimalStreamWidget(QWidget *parent) :
    QWidget(parent),
    peakProgressBar(new QProgressBar(this)),
    peakAnimation(new QPropertyAnimation(peakProgressBar, "value", peakProgressBar)),
    peak(nullptr),
    updating(false),
    volumeMeterEnabled(false),
    volumeMeterVisible(true) {

    peakProgressBar->setMaximum(1000);
    peakProgressBar->setTextVisible(false);
    peakProgressBar->hide();
    peakAnimation->setDuration(150);
}

void MinimalStreamWidget::initPeakProgressBar(QGridLayout* channelsGrid) {
    channelsGrid->addWidget(peakProgressBar, channelsGrid->rowCount(), 0, 1, -1);
}

void MinimalStreamWidget::updatePeak(double v) {
    int value = 0;
    if (v > 0) {
        peakProgressBar->setEnabled(true);
        value = qRound(v * peakProgressBar->maximum());
    } else {
        peakProgressBar->setEnabled(false);
    }
    peakAnimation->stop();
    if (peakProgressBar->value() > value)
    {
        peakAnimation->setStartValue(peakProgressBar->value());
        peakAnimation->setEndValue(value);
        peakAnimation->start();
    } else
    {
        peakProgressBar->setValue(value);
    }

    enableVolumeMeter();
}

void MinimalStreamWidget::enableVolumeMeter() {
    if (volumeMeterEnabled)
        return;

    volumeMeterEnabled = true;
    if (volumeMeterVisible) {
        peakProgressBar->show();
    }
}

void MinimalStreamWidget::setVolumeMeterVisible(bool v) {
    volumeMeterVisible = v;
    if (v) {
        if (volumeMeterEnabled) {
            peakProgressBar->show();
        }
    } else {
        peakProgressBar->hide();
    }
}
