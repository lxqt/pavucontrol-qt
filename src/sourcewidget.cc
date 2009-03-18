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

#include "sourcewidget.h"

#include "i18n.h"

SourceWidget::SourceWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    StreamWidget(cobject, x),
    defaultMenuItem(_("_Default"), true){

    add_events(Gdk::BUTTON_PRESS_MASK);

    defaultMenuItem.set_active(false);
    defaultMenuItem.signal_toggled().connect(sigc::mem_fun(*this, &SourceWidget::onDefaultToggle));
    menu.append(defaultMenuItem);
    menu.show_all();
}

SourceWidget* SourceWidget::create() {
    SourceWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "streamWidget");
    x->get_widget_derived("streamWidget", w);
    return w;
}

void SourceWidget::executeVolumeUpdate() {
    pa_operation* o;

    if (!(o = pa_context_set_source_volume_by_index(get_context(), index, &volume, NULL, NULL))) {
        show_error(_("pa_context_set_source_volume_by_index() failed"));
        return;
    }

    pa_operation_unref(o);
}

void SourceWidget::onMuteToggleButton() {
    StreamWidget::onMuteToggleButton();

    if (updating)
        return;

    pa_operation* o;
    if (!(o = pa_context_set_source_mute_by_index(get_context(), index, muteToggleButton->get_active(), NULL, NULL))) {
        show_error(_("pa_context_set_source_mute_by_index() failed"));
        return;
    }

    pa_operation_unref(o);
}

void SourceWidget::onDefaultToggle() {
    pa_operation* o;

    if (updating)
        return;

    if (!(o = pa_context_set_default_source(get_context(), name.c_str(), NULL, NULL))) {
        show_error(_("pa_context_set_default_source() failed"));
        return;
    }
    pa_operation_unref(o);
}
