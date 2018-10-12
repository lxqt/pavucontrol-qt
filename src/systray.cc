#include "systray.h"
#include "channel.h"
#include "sinkwidget.h"

Systray::Systray(MainWindow *parent) :
    mw(parent),
    muted(false) {

    systrayQuitAction.setText(tr("&Quit"));
    systrayRestoreAction.setText(tr("&Restore"));
    systrayMinimizeAction.setText(tr("Mi&nimize"));

    connect(&systrayMinimizeAction, &QAction::triggered, mw, &MainWindow::hide);
    connect(&systrayRestoreAction, &QAction::triggered, mw, &MainWindow::showNormal);
    connect(&systrayQuitAction, &QAction::triggered, mw, &MainWindow::quit);
    connect(this, &QSystemTrayIcon::activated, this, &Systray::iconActivated);

    systrayIconMenu.addAction(&systrayMinimizeAction);
    systrayIconMenu.addAction(&systrayRestoreAction);
    systrayIconMenu.addSeparator(); 
    systrayIconMenu.addAction(&systrayQuitAction);

    QSize sz(256,256);
    systrayIcons[NOT_MUTED].addPixmap(mw->style()->standardIcon(QStyle::SP_MediaVolume).pixmap(sz));
    systrayIcons[MUTED].addPixmap(mw->style()->standardIcon(QStyle::SP_MediaVolumeMuted).pixmap(sz));

    setIcon(systrayIcons[NOT_MUTED]);
    setContextMenu(&systrayIconMenu);
    show();
}

void Systray::muteToggle()
{
    static std::map<uint32_t, bool> restoreMuteState;
    for (std::map<uint32_t, SinkWidget*>::iterator it = mw->sinkWidgets.begin(); it != mw->sinkWidgets.end(); ++it) {
        QToolButton *sinkMuteToggleButton = it->second->muteToggleButton;
        if(!muted) {
            restoreMuteState[it->first] =  sinkMuteToggleButton->isChecked();
            sinkMuteToggleButton->setChecked(true);
        } else {
            if(restoreMuteState.find(it->first) != restoreMuteState.end())
                sinkMuteToggleButton->setChecked(restoreMuteState[it->first]);
        }
    }
    muted = !muted;
    setIcon(systrayIcons[muted ? MUTED : NOT_MUTED]);
}

void Systray::iconActivated(QSystemTrayIcon::ActivationReason reason){
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        mw->setVisible(!mw->isVisible());
        break;
    case QSystemTrayIcon::MiddleClick:
        muteToggle();
        break;
    default:
        break;
    }
}

void Systray::setVisible(bool visible) {
    systrayMinimizeAction.setEnabled(visible);
    systrayRestoreAction.setEnabled(!visible);
}

void Systray::volumeChange(int step) {
    for (std::map<uint32_t, SinkWidget*>::iterator it = mw->sinkWidgets.begin(); it != mw->sinkWidgets.end(); ++it) {
        DeviceWidget* w = dynamic_cast<DeviceWidget*>(it->second);
        if(!w || !w->channels || !w->channels[0])
            continue;

        Channel *c = w->channels[0];
        QSlider *vs = c->volumeScale;
        int vol = vs->value();
        int max = vs->maximum();
        int min = vs->minimum();
        double single_percent_step = (double)(max-min) / 100;
        vs->setValue(vol + step*single_percent_step);
    }
}

bool Systray::eventFilter(QObject *obj, QEvent *event) {
    if(obj == this && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
        if(wheelEvent->delta() > 0)
            volumeChange(5);
        else
            volumeChange(-5);
    }
    return false;
}

