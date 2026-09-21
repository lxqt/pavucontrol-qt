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
  along with pavucontrol. If not, see <https://www.gnu.org/licenses/>.
***/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pavucontrol.h"
#include "cardwidget.h"

/*** CardWidget ***/
CardWidget::CardWidget(QWidget* parent) :
    QWidget(parent) {
    setupUi(this);
    connect(profileList, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &CardWidget::onProfileChange);
    connect(profileCB, &QAbstractButton::toggled, this, &CardWidget::onProfileCheck);
    connect(codecList, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &CardWidget::onCodecChange);
    /* Hidden until prepareMenu() finds this card actually has codecs to
     * offer - most cards never will, since this is Bluetooth-only. */
    codecBox->hide();
}


void CardWidget::prepareMenu() {
    int idx = 0;
    const bool off = activeProfile == noInOutProfile;
    /* Some backends (observed with PipeWire's own Bluetooth support, as
     * opposed to real PulseAudio's module-bluez5-device) expose codec choice
     * as separate profile entries instead - e.g. "High Fidelity Playback
     * (A2DP Sink, codec LDAC)" *and* "... codec AAC)" *and* "... codec SBC)"
     * all appearing as distinct, individually selectable profiles. When
     * that's the case, the Profile dropdown below already IS the codec
     * switcher, and showing our own separate Codec dropdown alongside it
     * would just be a confusing, redundant second control offering the same
     * choice. Real PulseAudio only ever exposes a single, codec-agnostic
     * A2DP profile - "High Fidelity Playback (A2DP Sink)", no codec name in
     * sight - which is the actual case this feature exists for. */
    bool profileListAlreadyOffersCodecs = false;

    profileList->clear();
    /* Fill the ComboBox */
    for (const auto & profile : profiles) {
        QByteArray name = profile.first;
        // skip the "off" profile
        if (name == noInOutProfile)
            continue;
        if (profile.second.contains(", codec "))
            profileListAlreadyOffersCodecs = true;
        QString desc = QString::fromUtf8(profile.second);
        profileList->addItem(desc, name);
        if (profile.first == activeProfile
                || (off && profile.first == lastActiveProfile)
                )
        {
            profileList->setCurrentIndex(idx);
            lastActiveProfile = profile.first;
        }
        ++idx;
    }

    profileCB->setChecked(!off);

    /* Same fill-and-select pattern as the profile combo box above, but for
     * Bluetooth codecs. `codecs` (and therefore this whole block) stays
     * empty for every non-Bluetooth card, in which case the loop does
     * nothing and codecBox ends up hidden below - exactly the same as
     * before this feature existed. Also stays empty when the profile list
     * already offers per-codec entries, per the comment above. */
    codecList->clear();
    if (!profileListAlreadyOffersCodecs) {
        for (const auto & codec : codecs) {
            codecList->addItem(QString::fromUtf8(codec.second), codec.first);
            if (codec.first == activeCodec)
                codecList->setCurrentIndex(codecList->count() - 1);
        }
    }
    codecBox->setVisible(!profileListAlreadyOffersCodecs && !codecs.empty());
}

void CardWidget::changeProfile(const QByteArray & name)
{
    pa_operation* o;

    if (!(o = pa_context_set_card_profile_by_index(get_context(), index, name.constData(), nullptr, nullptr))) {
        show_error(tr("pa_context_set_card_profile_by_index() failed").toUtf8().constData());
        return;
    }

    pa_operation_unref(o);
}

void CardWidget::onProfileChange(int active) {
    if (updating)
        return;

    if (active != -1)
        changeProfile(profileList->itemData(active).toByteArray());
}

void CardWidget::onProfileCheck(bool on)
{
    if (updating)
        return;

    if (on)
        onProfileChange(profileList->currentIndex());
    else
        changeProfile(noInOutProfile);

}

#if HAVE_PULSE_MESSAGING_API
void CardWidget::onCodecChange(int active) {
    pa_operation* o;

    if (updating || active == -1)
        return;

    QByteArray codecName = codecList->itemData(active).toByteArray();
    /* The "switch-codec" message's parameter must be the codec name encoded
     * as a bare JSON string, e.g. `"ldac"` (quotes included) - see the
     * protocol comment above context_message_handlers_cb() in pavucontrol.cc.
     * Codec names come back-and-forth verbatim from PulseAudio itself
     * (they're short fixed identifiers like "sbc"/"ldac"/"aptx", never
     * user-supplied text), so no JSON escaping is needed here. */
    QByteArray params = "\"" + codecName + "\"";

    /* Deliberately does NOT call show_error() on failure: show_error() calls
     * qApp->quit(), which is right for genuinely fatal errors elsewhere in
     * this codebase but would be a wildly disproportionate response to a
     * rejected codec switch (e.g. the headset doesn't actually support the
     * chosen codec, or got disconnected mid-switch) - the user would just
     * lose the whole app over a dropdown selection. Codec switching failing
     * silently here matches how the discovery queries in pavucontrol.cc
     * already treat every failure in this feature as non-fatal. */
    if (!(o = pa_context_send_message_to_object(get_context(), bluezMessageHandlerPath(pulseCardName).constData(),
            "switch-codec", params.constData(), nullptr, nullptr)))
        return;

    pa_operation_unref(o);
}
#else
void CardWidget::onCodecChange(int) {
}
#endif
