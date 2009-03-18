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

#include "sinkwidget.h"

#include "i18n.h"

SinkWidget::SinkWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    StreamWidget(cobject, x),
    defaultMenuItem("_Default", true){

    add_events(Gdk::BUTTON_PRESS_MASK);

    defaultMenuItem.set_active(false);
    defaultMenuItem.signal_toggled().connect(sigc::mem_fun(*this, &SinkWidget::onDefaultToggle));
    menu.append(defaultMenuItem);
    menu.show_all();
}

SinkWidget* SinkWidget::create() {
    SinkWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    return w;
}

void SinkWidget::executeVolumeUpdate() {
    pa_operation* o;

    if (!(o = pa_context_set_sink_volume_by_index(get_context(), index, &volume, NULL, NULL))) {
        show_error(_("pa_context_set_sink_volume_by_index() failed"));
        return;
    }

    pa_operation_unref(o);
}

void SinkWidget::onMuteToggleButton() {
    StreamWidget::onMuteToggleButton();

    if (updating)
        return;

    pa_operation* o;
    if (!(o = pa_context_set_sink_mute_by_index(get_context(), index, muteToggleButton->get_active(), NULL, NULL))) {
        show_error(_("pa_context_set_sink_mute_by_index() failed"));
        return;
    }

    pa_operation_unref(o);
}

void SinkWidget::onDefaultToggle() {
    pa_operation* o;

    if (updating)
        return;

    if (!(o = pa_context_set_default_sink(get_context(), name.c_str(), NULL, NULL))) {
        show_error(_("pa_context_set_default_sink() failed"));
        return;
    }
    pa_operation_unref(o);
}
