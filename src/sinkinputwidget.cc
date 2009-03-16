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

#include "i18n.h"

SinkInputWidget::SinkInputWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    StreamWidget(cobject, x),
    mpMainWindow(NULL) {

    directionLabel->set_label(_("<i>Playing on </i> "));
}

void SinkInputWidget::init(MainWindow* mainWindow) {
    mpMainWindow = mainWindow;
    deviceCombo->set_model(mpMainWindow->sinkTree);
    deviceCombo->pack_start(mpMainWindow->deviceColumns.name);
}

SinkInputWidget* SinkInputWidget::create(MainWindow* mainWindow) {
    SinkInputWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    w->init(mainWindow);
    return w;
}

void SinkInputWidget::setSinkIndex(uint32_t idx) {
    mSinkIndex = idx;

    mSuppressDeviceChange = true;
    deviceCombo->set_active(mpMainWindow->sinkTreeIndexes[idx]);
    mSuppressDeviceChange = false;
}

uint32_t SinkInputWidget::sinkIndex() {
    return mSinkIndex;
}

void SinkInputWidget::executeVolumeUpdate() {
    pa_operation* o;

    if (!(o = pa_context_set_sink_input_volume(get_context(), index, &volume, NULL, NULL))) {
        show_error(_("pa_context_set_sink_input_volume() failed"));
        return;
    }

    pa_operation_unref(o);
}

void SinkInputWidget::onMuteToggleButton() {
    StreamWidget::onMuteToggleButton();

    if (updating)
        return;

    pa_operation* o;
    if (!(o = pa_context_set_sink_input_mute(get_context(), index, muteToggleButton->get_active(), NULL, NULL))) {
        show_error(_("pa_context_set_sink_input_mute() failed"));
        return;
    }

    pa_operation_unref(o);
}

void SinkInputWidget::onKill() {
    pa_operation* o;
    if (!(o = pa_context_kill_sink_input(get_context(), index, NULL, NULL))) {
        show_error(_("pa_context_kill_sink_input() failed"));
        return;
    }

    pa_operation_unref(o);
}

void SinkInputWidget::onDeviceChange() {
    Gtk::TreeModel::iterator iter;

    if (updating || mSuppressDeviceChange)
        return;

    iter = deviceCombo->get_active();
    if (iter)
    {
        Gtk::TreeModel::Row row = *iter;
        if (row)
        {
          pa_operation* o;
          uint32_t sink_index = row[mpMainWindow->deviceColumns.index];

          if (!(o = pa_context_move_sink_input_by_index(get_context(), index, sink_index, NULL, NULL))) {
              show_error(_("pa_context_move_sink_input_by_index() failed"));
              return;
          }

          pa_operation_unref(o);
        }
    }
}
