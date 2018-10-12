#ifndef systray_h
#define systray_h

#include <QMenu>
#include <QSystemTrayIcon>
#include "mainwindow.h"
#include "sinkwidget.h"
#include "channel.h"

class MainWindow;
class DeviceWidget;
class SinkWidget;

struct Systray : public QSystemTrayIcon {
    Systray(MainWindow *parent);

    bool eventFilter(QObject *obj, QEvent *event);
    void volumeChange(int step);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void muteToggle();
    void setVisible(bool visible);

private:
    MainWindow *mw;
    enum { NOT_MUTED, MUTED };
    bool muted;
    QIcon systrayIcons[2];
    QMenu systrayIconMenu;
    QAction systrayMinimizeAction;
    QAction systrayRestoreAction;
    QAction systrayQuitAction;
};

#endif
