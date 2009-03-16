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
    titleMenuItem(_("_Move Stream..."), true),
    killMenuItem(_("_Terminate Stream"), true),
    mpMainWindow(NULL) {

    directionLabel->set_label(_("<i>Recording from </i> "));

    add_events(Gdk::BUTTON_PRESS_MASK);

    menu.append(titleMenuItem);
    titleMenuItem.set_submenu(submenu);

    menu.append(killMenuItem);
    killMenuItem.signal_activate().connect(sigc::mem_fun(*this, &SourceOutputWidget::onKill));
}

void SourceOutputWidget::init(MainWindow* mainWindow) {
    mpMainWindow = mainWindow;
    deviceCombo->set_model(mpMainWindow->sourceTree);
    deviceCombo->pack_start(mpMainWindow->deviceColumns.name);
}

SourceOutputWidget::~SourceOutputWidget() {
    clearMenu();
}

SourceOutputWidget* SourceOutputWidget::create(MainWindow* mainWindow) {
    SourceOutputWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    w->init(mainWindow);
    return w;
}

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
        sourceMenuItems[i->second->index] = m = new SourceMenuItem(this, i->second->description.c_str(), i->second->index, i->second->index == sourceIndex);
        submenu.append(m->menuItem);
    }

    menu.show_all();
}

void SourceOutputWidget::prepareMenu(void) {
  clearMenu();
  buildMenu();
}

void SourceOutputWidget::onDeviceChange() {
}

void SourceOutputWidget::SourceMenuItem::onToggle() {

    if (widget->updating)
        return;

    if (!menuItem.get_active())
        return;

    pa_operation* o;
    if (!(o = pa_context_move_source_output_by_index(get_context(), widget->index, index, NULL, NULL))) {
        show_error(_("pa_context_move_source_output_by_index() failed"));
        return;
    }

    pa_operation_unref(o);
}
