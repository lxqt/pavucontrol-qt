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

#include "sourceoutputwidget.h"
#include "mainwindow.h"
#include "sourcewidget.h"

#include "i18n.h"

SourceOutputWidget::SourceOutputWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    StreamWidget(cobject, x),
    mpMainWindow(NULL) {

    directionLabel->set_label(_("<i>Recording from </i> "));
}

void SourceOutputWidget::init(MainWindow* mainWindow) {
    mpMainWindow = mainWindow;
    deviceCombo->set_model(mpMainWindow->sourceTree);
    deviceCombo->pack_start(mpMainWindow->deviceColumns.name);
}

SourceOutputWidget* SourceOutputWidget::create(MainWindow* mainWindow) {
    SourceOutputWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    w->init(mainWindow);
    return w;
}

void SourceOutputWidget::setSourceIndex(uint32_t idx) {
    mSourceIndex = idx;

    mSuppressDeviceChange = true;
    deviceCombo->set_active(mpMainWindow->sourceTreeIndexes[idx]);
    mSuppressDeviceChange = false;
}

uint32_t SourceOutputWidget::sourceIndex() {
    return mSourceIndex;
}

void SourceOutputWidget::onKill() {
    pa_operation* o;
    if (!(o = pa_context_kill_source_output(get_context(), index, NULL, NULL))) {
        show_error(_("pa_context_kill_source_output() failed"));
        return;
    }

    pa_operation_unref(o);
}

void SourceOutputWidget::onDeviceChange() {
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
          uint32_t source_index = row[mpMainWindow->deviceColumns.index];

          if (!(o = pa_context_move_source_output_by_index(get_context(), source_index, index, NULL, NULL))) {
              show_error(_("pa_context_move_source_output_by_index() failed"));
              return;
          }

          pa_operation_unref(o);
        }
    }
}
