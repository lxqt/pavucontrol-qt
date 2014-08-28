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

SourceOutputWidget::SourceOutputWidget(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& x) :
    StreamWidget(cobject, x) {

    gchar *txt;
    directionLabel->set_label(txt = g_markup_printf_escaped("<i>%s</i>", _("from")));
    g_free(txt);

    terminate.set_label(_("Terminate Recording"));

#if !HAVE_SOURCE_OUTPUT_VOLUMES
    /* Source Outputs do not have volume controls in versions of PA < 1.0 */
    muteToggleButton->hide();
    lockToggleButton->hide();
#endif
}

SourceOutputWidget* SourceOutputWidget::create(MainWindow* mainWindow) {
    SourceOutputWidget* w;
    Glib::RefPtr<Gtk::Builder> x = Gtk::Builder::create_from_file(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    w->init(mainWindow);
    w->reference();
    return w;
}

SourceOutputWidget::~SourceOutputWidget(void) {
  clearMenu();
}

void SourceOutputWidget::setSourceIndex(uint32_t idx) {
    mSourceIndex = idx;

    if (mpMainWindow->sourceWidgets.count(idx)) {
      SourceWidget *w = mpMainWindow->sourceWidgets[idx];
      deviceButton->set_label(w->description.c_str());
    }
    else
      deviceButton->set_label(_("Unknown input"));
}

uint32_t SourceOutputWidget::sourceIndex() {
    return mSourceIndex;
}

#if HAVE_SOURCE_OUTPUT_VOLUMES
void SourceOutputWidget::executeVolumeUpdate() {
    pa_operation* o;

    if (!(o = pa_context_set_source_output_volume(get_context(), index, &volume, NULL, NULL))) {
        show_error(_("pa_context_set_source_output_volume() failed"));
        return;
    }

    pa_operation_unref(o);
}

void SourceOutputWidget::onMuteToggleButton() {
    StreamWidget::onMuteToggleButton();

    if (updating)
        return;

    pa_operation* o;
    if (!(o = pa_context_set_source_output_mute(get_context(), index, muteToggleButton->get_active(), NULL, NULL))) {
        show_error(_("pa_context_set_source_output_mute() failed"));
        return;
    }

    pa_operation_unref(o);
}
#endif

void SourceOutputWidget::onKill() {
    pa_operation* o;
    if (!(o = pa_context_kill_source_output(get_context(), index, NULL, NULL))) {
        show_error(_("pa_context_kill_source_output() failed"));
        return;
    }

    pa_operation_unref(o);
}


void SourceOutputWidget::clearMenu() {
  while (!sourceMenuItems.empty()) {
    std::map<uint32_t, SourceMenuItem*>::iterator i = sourceMenuItems.begin();
    delete i->second;
    sourceMenuItems.erase(i);
  }
}

void SourceOutputWidget::buildMenu() {
  for (std::map<uint32_t, SourceWidget*>::iterator i = mpMainWindow->sourceWidgets.begin(); i != mpMainWindow->sourceWidgets.end(); ++i) {
    SourceMenuItem *m;
    sourceMenuItems[i->second->index] = m = new SourceMenuItem(this, i->second->description.c_str(), i->second->index, i->second->index == mSourceIndex);
    menu.append(m->menuItem);
  }
  menu.show_all();
}

void SourceOutputWidget::SourceMenuItem::onToggle() {
  if (widget->updating)
    return;

  if (!menuItem.get_active())
    return;

  /*if (!mpMainWindow->sourceWidgets.count(widget->index))
    return;*/

  pa_operation* o;
  if (!(o = pa_context_move_source_output_by_index(get_context(), widget->index, index, NULL, NULL))) {
    show_error(_("pa_context_move_source_output_by_index() failed"));
    return;
  }

  pa_operation_unref(o);
}

void SourceOutputWidget::onDeviceChangePopup() {
    clearMenu();
    buildMenu();
    menu.popup(1, 0);
}
