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

#include "minimalstreamwidget.h"

/*** MinimalStreamWidget ***/
MinimalStreamWidget::MinimalStreamWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    Gtk::VBox(cobject),
    peakProgressBar(),
    lastPeak(0),
    updating(false),
    volumeMeterEnabled(false) {

    x->get_widget("channelsVBox", channelsVBox);
    x->get_widget("nameLabel", nameLabel);
    x->get_widget("boldNameLabel", boldNameLabel);
    x->get_widget("streamToggle", streamToggleButton);
    x->get_widget("iconImage", iconImage);

    peakProgressBar.set_size_request(-1, 10);
    channelsVBox->pack_end(peakProgressBar, false, false);

    streamToggleButton->set_active(false);
    streamToggleButton->signal_clicked().connect(sigc::mem_fun(*this, &MinimalStreamWidget::onStreamToggleButton));
    menu.signal_deactivate().connect(sigc::mem_fun(*this, &MinimalStreamWidget::onMenuDeactivated));

    peakProgressBar.hide();
}

void MinimalStreamWidget::prepareMenu(void) {
}

void MinimalStreamWidget::onMenuDeactivated(void) {
    streamToggleButton->set_active(false);
}

void MinimalStreamWidget::popupMenuPosition(int& x, int& y, bool& push_in G_GNUC_UNUSED) {
    Gtk::Requisition  r;

    streamToggleButton->get_window()->get_origin(x, y);
    r = menu.size_request();

    /* Align the right side of the menu with the right side of the togglebutton */
    x += streamToggleButton->get_allocation().get_x();
    x += streamToggleButton->get_allocation().get_width();
    x -= r.width;

    /* Align the top of the menu with the buttom of the togglebutton */
    y += streamToggleButton->get_allocation().get_y();
    y += streamToggleButton->get_allocation().get_height();
}

void MinimalStreamWidget::onStreamToggleButton(void) {
    if (streamToggleButton->get_active()) {
        prepareMenu();
        menu.popup(sigc::mem_fun(*this, &MinimalStreamWidget::popupMenuPosition), 0, gtk_get_current_event_time());
    }
}

bool MinimalStreamWidget::on_button_press_event (GdkEventButton* event) {
    if (Gtk::VBox::on_button_press_event(event))
        return TRUE;

    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        prepareMenu();
        menu.popup(0, event->time);
        return TRUE;
    }

    return FALSE;
}

#define DECAY_STEP .04

void MinimalStreamWidget::updatePeak(double v) {

    if (lastPeak >= DECAY_STEP)
        if (v < lastPeak - DECAY_STEP)
            v = lastPeak - DECAY_STEP;

    lastPeak = v;

    if (v >= 0) {
        peakProgressBar.set_sensitive(TRUE);
        peakProgressBar.set_fraction(v);
    } else {
        peakProgressBar.set_sensitive(FALSE);
        peakProgressBar.set_fraction(0);
    }

    enableVolumeMeter();
}

void MinimalStreamWidget::enableVolumeMeter() {
    if (volumeMeterEnabled)
        return;

    volumeMeterEnabled = true;
    peakProgressBar.show();
}

