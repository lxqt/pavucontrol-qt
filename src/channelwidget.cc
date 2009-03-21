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

#include "channelwidget.h"
#include "streamwidget.h"

#include "i18n.h"

static bool show_decibel = true;

/*** ChannelWidget ***/

ChannelWidget::ChannelWidget(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& x) :
    Gtk::EventBox(cobject),
    volumeScaleEnabled(true) {

    x->get_widget("channelLabel", channelLabel);
    x->get_widget("volumeLabel", volumeLabel);
    x->get_widget("volumeScale", volumeScale);

    volumeScale->set_value(100.0);
    volumeScale->set_increments(100.0/PA_VOLUME_NORM, 100.0/PA_VOLUME_NORM);

    volumeScale->signal_value_changed().connect(sigc::mem_fun(*this, &ChannelWidget::onVolumeScaleValueChanged));
}

ChannelWidget* ChannelWidget::create() {
    ChannelWidget* w;
    Glib::RefPtr<Gnome::Glade::Xml> x = Gnome::Glade::Xml::create(GLADE_FILE, "channelWidget");
    x->get_widget_derived("channelWidget", w);
    return w;
}

void ChannelWidget::setVolume(pa_volume_t volume) {
    double v;
    char txt[64];

    v = ((gdouble) volume * 100) / PA_VOLUME_NORM;

    if (can_decibel && show_decibel) {
        double dB = pa_sw_volume_to_dB(volume);

        if (dB > PA_DECIBEL_MININFTY) {
            snprintf(txt, sizeof(txt), "%0.2f dB", dB);
            volumeLabel->set_tooltip_text(txt);
        } else
            volumeLabel->set_tooltip_markup("-&#8734;dB");
        volumeLabel->set_has_tooltip(TRUE);
    } else
        volumeLabel->set_has_tooltip(FALSE);

    snprintf(txt, sizeof(txt), "%0.0f%%", v);
    volumeLabel->set_text(txt);

    volumeScaleEnabled = false;
    volumeScale->set_value(v > 100 ? 100 : v);
    volumeScaleEnabled = true;
}

void ChannelWidget::onVolumeScaleValueChanged() {

    if (!volumeScaleEnabled)
        return;

    if (streamWidget->updating)
        return;

    pa_volume_t volume = (pa_volume_t) ((volumeScale->get_value() * PA_VOLUME_NORM) / 100);
    streamWidget->updateChannelVolume(channel, volume);

    if (beepDevice != "") {
        ca_context_change_device(ca_gtk_context_get(), beepDevice.c_str());

        ca_context_cancel(ca_gtk_context_get(), 2);

        ca_gtk_play_for_widget(GTK_WIDGET(volumeScale->gobj()),
                               2,
                               CA_PROP_EVENT_DESCRIPTION, _("Volume Control Feedback Sound"),
                               CA_PROP_EVENT_ID, "audio-volume-change",
                               CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
                               CA_PROP_CANBERRA_VOLUME, "0",
                               CA_PROP_CANBERRA_ENABLE, "1",
                               NULL);

        ca_context_change_device(ca_gtk_context_get(), NULL);
    }
}

void ChannelWidget::set_sensitive(bool enabled) {
    Gtk::EventBox::set_sensitive(enabled);

    channelLabel->set_sensitive(enabled);
    volumeLabel->set_sensitive(enabled);
    volumeScale->set_sensitive(enabled);
}

void ChannelWidget::setBaseVolume(pa_volume_t v) {

    gtk_scale_add_mark(GTK_SCALE(volumeScale->gobj()), 0.0, (GtkPositionType) GTK_POS_BOTTOM, _("<small>Silence</small>"));
    gtk_scale_add_mark(GTK_SCALE(volumeScale->gobj()), 100.0, (GtkPositionType) GTK_POS_BOTTOM, _("<small>Max</small>"));

    if (v > PA_VOLUME_MUTED && v < PA_VOLUME_NORM) {
        double p = ((double) v * 100) / PA_VOLUME_NORM;
        gtk_scale_add_mark(GTK_SCALE(volumeScale->gobj()), p, (GtkPositionType) GTK_POS_BOTTOM, _("<small><i>Base</i></small>"));
    }

}

void ChannelWidget::setSteps(unsigned n) {
    volumeScale->set_increments(100.0/(n-1), 100.0/(n-1));
}
