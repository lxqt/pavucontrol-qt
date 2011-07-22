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

#include <canberra-gtk.h>

#include "sinkwidget.h"

#include "i18n.h"

SinkWidget::SinkWidget(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& x) :
    DeviceWidget(cobject, x) {
}

SinkWidget* SinkWidget::create(MainWindow* mainWindow) {
    SinkWidget* w;
    Glib::RefPtr<Gtk::Builder> x = Gtk::Builder::create_from_file(GLADE_FILE, "deviceWidget");
    x->get_widget_derived("deviceWidget", w);
    w->init(mainWindow, "sink");
    return w;
}

void SinkWidget::executeVolumeUpdate() {
    pa_operation* o;
    char dev[64];
    int playing = 0;

    if (!(o = pa_context_set_sink_volume_by_index(get_context(), index, &volume, NULL, NULL))) {
        show_error(_("pa_context_set_sink_volume_by_index() failed"));
        return;
    }

    pa_operation_unref(o);

    ca_context_playing(ca_gtk_context_get(), 2, &playing);
    if (playing)
        return;

    snprintf(dev, sizeof(dev), "%lu", (unsigned long) index);
    ca_context_change_device(ca_gtk_context_get(), dev);

    ca_gtk_play_for_widget(GTK_WIDGET(gobj()),
                           2,
                           CA_PROP_EVENT_DESCRIPTION, _("Volume Control Feedback Sound"),
                           CA_PROP_EVENT_ID, "audio-volume-change",
                           CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
                           CA_PROP_CANBERRA_ENABLE, "1",
                           NULL);

    ca_context_change_device(ca_gtk_context_get(), NULL);
}

void SinkWidget::onMuteToggleButton() {
    DeviceWidget::onMuteToggleButton();

    if (updating)
        return;

    pa_operation* o;
    if (!(o = pa_context_set_sink_mute_by_index(get_context(), index, muteToggleButton->get_active(), NULL, NULL))) {
        show_error(_("pa_context_set_sink_mute_by_index() failed"));
        return;
    }

    pa_operation_unref(o);
}

void SinkWidget::onDefaultToggleButton() {
    pa_operation* o;

    if (updating)
        return;

    if (!(o = pa_context_set_default_sink(get_context(), name.c_str(), NULL, NULL))) {
        show_error(_("pa_context_set_default_sink() failed"));
        return;
    }
    pa_operation_unref(o);
}

void SinkWidget::onPortChange() {
    Gtk::TreeModel::iterator iter;

    if (updating)
        return;

    iter = portList->get_active();
    if (iter) {
        Gtk::TreeModel::Row row = *iter;
        if (row) {
            pa_operation* o;
            Glib::ustring port = row[portModel.name];

            if (!(o = pa_context_set_sink_port_by_index(get_context(), index, port.c_str(), NULL, NULL))) {
                show_error(_("pa_context_set_sink_port_by_index() failed"));
                return;
            }

            pa_operation_unref(o);
        }
    }
}
