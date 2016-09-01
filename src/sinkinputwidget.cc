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

#include "sinkinputwidget.h"
#include "mainwindow.h"
#include "sinkwidget.h"


SinkInputWidget::SinkInputWidget(MainWindow *parent) :
    StreamWidget(parent) {

    gchar *txt;
    directionLabel->setText(QString::fromUtf8(txt = g_markup_printf_escaped("<i>%s</i>", tr("on").toUtf8().constData())));
    g_free(txt);

    // terminate.setText(tr("Terminate Playback").toUtf8().constData());
}

SinkInputWidget::~SinkInputWidget(void) {
    clearMenu();
}

void SinkInputWidget::setSinkIndex(uint32_t idx) {
    mSinkIndex = idx;

    if (mpMainWindow->sinkWidgets.count(idx)) {
        SinkWidget *w = mpMainWindow->sinkWidgets[idx];
        deviceButton->setText(w->description);
    }
    else
        deviceButton->setText(tr("Unknown output").toUtf8().constData());
}

uint32_t SinkInputWidget::sinkIndex() {
    return mSinkIndex;
}

void SinkInputWidget::executeVolumeUpdate() {
    pa_operation* o;

    if (!(o = pa_context_set_sink_input_volume(get_context(), index, &volume, NULL, NULL))) {
        show_error(tr("pa_context_set_sink_input_volume() failed").toUtf8().constData());
        return;
    }

    pa_operation_unref(o);
}

void SinkInputWidget::onMuteToggleButton() {
    StreamWidget::onMuteToggleButton();

    if (updating)
        return;

    pa_operation* o;
    if (!(o = pa_context_set_sink_input_mute(get_context(), index, muteToggleButton->isChecked(), NULL, NULL))) {
        show_error(tr("pa_context_set_sink_input_mute() failed").toUtf8().constData());
        return;
    }

    pa_operation_unref(o);
}

void SinkInputWidget::onKill() {
    pa_operation* o;
    if (!(o = pa_context_kill_sink_input(get_context(), index, NULL, NULL))) {
        show_error(tr("pa_context_kill_sink_input() failed").toUtf8().constData());
        return;
    }

    pa_operation_unref(o);
}

void SinkInputWidget::clearMenu() {
#if 0
  while (!sinkMenuItems.empty()) {
    std::map<uint32_t, SinkMenuItem*>::iterator i = sinkMenuItems.begin();
    delete i->second;
    sinkMenuItems.erase(i);
  }
#endif
}

void SinkInputWidget::buildMenu() {
#if 0
  for (std::map<uint32_t, SinkWidget*>::iterator i = mpMainWindow->sinkWidgets.begin(); i != mpMainWindow->sinkWidgets.end(); ++i) {
    SinkMenuItem *m;
    sinkMenuItems[i->second->index] = m = new SinkMenuItem(this, i->second->description, i->second->index, i->second->index == mSinkIndex);
    menu.append(m->menuItem);
  }
  menu.show_all();
#endif
}

#if 0
void SinkInputWidget::SinkMenuItem::onToggle() {
  if (widget->updating)
    return;

  if (!menuItem.get_active())
    return;

  /*if (!mpMainWindow->sinkWidgets.count(widget->index))
    return;*/

  pa_operation* o;
  if (!(o = pa_context_move_sink_input_by_index(get_context(), widget->index, index, NULL, NULL))) {
    show_error(tr("pa_context_move_sink_input_by_index() failed").toUtf8().constData());
    return;
  }

  pa_operation_unref(o);
}

#endif

void SinkInputWidget::onDeviceChangePopup() {
    clearMenu();
    buildMenu();
    // menu.popup(1, 0);
}
