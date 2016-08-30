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

#include "cardwidget.h"

#include "i18n.h"

/*** CardWidget ***/
CardWidget::CardWidget(QWidget* parent) :
    QWidget(parent) {
    ui.setupUi(this);
    // profileList->signal_changed().connect( sigc::mem_fun(*this, &CardWidget::onProfileChange));
}


void CardWidget::prepareMenu() {
    int idx = 0;
    int active_idx = -1;

    ui.profileList->clear();
    /* Fill the ComboBox */
    for (uint32_t i = 0; i < profiles.size(); ++i) {
        const auto& profile = profiles[i];
        QByteArray name = profile.first.c_str();
        QString desc = QString::fromUtf8(profile.second.c_str());
        ui.profileList->addItem(desc, name);
        if (profile.first == activeProfile)
          active_idx = idx;
        idx++;
    }

    if (active_idx >= 0)
        ui.profileList->setCurrentIndex(active_idx);

}

void CardWidget::onProfileChange() {
    if (updating)
        return;

    int active = ui.profileList->currentIndex();
    if (active != -1) {
        QString desc = ui.profileList->itemText(active);
        QByteArray name = ui.profileList->itemData(active).toByteArray();
        pa_operation* o;

        if (!(o = pa_context_set_card_profile_by_index(get_context(), index, name.constData(), NULL, NULL))) {
            show_error(_("pa_context_set_card_profile_by_index() failed"));
            return;
        }

        pa_operation_unref(o);
    }
}
