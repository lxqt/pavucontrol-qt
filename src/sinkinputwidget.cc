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
    mainWindow(NULL),
    titleMenuItem(_("_Move Stream..."), true),
    killMenuItem(_("_Terminate Stream"), true) {

    add_events(Gdk::BUTTON_PRESS_MASK);

    menu.append(titleMenuItem);
    titleMenuItem.set_submenu(submenu);

    menu.append(killMenuItem);
    killMenuItem.signal_activate().connect(sigc::mem_fun(*this, &SinkInputWidget::onKill));
}

SinkInputWidget::~SinkInputWidget() {
    clearMenu();
}

SinkInputWidget* SinkInputWidget::create() {
    SinkInputWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    return w;
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

void SinkInputWidget::prepareMenu() {
  clearMenu();
  buildMenu();
}

void SinkInputWidget::clearMenu() {

    while (!sinkMenuItems.empty()) {
        std::map<uint32_t, SinkMenuItem*>::iterator i = sinkMenuItems.begin();
        delete i->second;
        sinkMenuItems.erase(i);
    }
}

void SinkInputWidget::buildMenu() {
    for (std::map<uint32_t, SinkWidget*>::iterator i = mainWindow->sinkWidgets.begin(); i != mainWindow->sinkWidgets.end(); ++i) {
        SinkMenuItem *m;
        sinkMenuItems[i->second->index] = m = new SinkMenuItem(this, i->second->description.c_str(), i->second->index, i->second->index == sinkIndex);
        submenu.append(m->menuItem);
    }

    menu.show_all();
}

void SinkInputWidget::onKill() {
    pa_operation* o;
    if (!(o = pa_context_kill_sink_input(get_context(), index, NULL, NULL))) {
        show_error(_("pa_context_kill_sink_input() failed"));
        return;
    }

    pa_operation_unref(o);
}

void SinkInputWidget::SinkMenuItem::onToggle() {

    if (widget->updating)
        return;

    if (!menuItem.get_active())
        return;

    pa_operation* o;
    if (!(o = pa_context_move_sink_input_by_index(get_context(), widget->index, index, NULL, NULL))) {
        show_error(_("pa_context_move_sink_input_by_index() failed"));
        return;
    }

    pa_operation_unref(o);
}
