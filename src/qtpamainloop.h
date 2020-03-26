#pragma once

#include <QCoreApplication>
#include <QTimer>
#include <QSocketNotifier>
#include <QDateTime>
#include <QDebug>
#include <pulse/mainloop-api.h>

struct QtPaMainLoop {
    pa_mainloop_api pa_vtable{};

    QtPaMainLoop()
    {
        pa_vtable.userdata = this;

        pa_vtable.io_new = newIoEvent;
        pa_vtable.io_enable = setIoEnabled;
        pa_vtable.io_free = ioDestroy;
        pa_vtable.io_set_destroy = setIoDestructor;
        pa_vtable.time_new = newTimer;
        pa_vtable.time_restart = restartTimer;
        pa_vtable.time_free = freeTimer;
        pa_vtable.time_set_destroy = timerSetDestructor;
        pa_vtable.time_set_destroy = timerSetDestructor;

        pa_vtable.defer_new = newDefer;
        pa_vtable.defer_enable = setDeferEnabled;
        pa_vtable.defer_free = freeDefer;
        pa_vtable.defer_set_destroy = deferSetDestructor;

        pa_vtable.quit = quit;
    }

    static int msecsUntilTimeval(const struct timeval *tv)
    {
        time_t target = tv->tv_sec * 1000;

        if (tv->tv_usec) {
            target += tv->tv_usec / 1000;
        }

        return QDateTime::currentDateTime().msecsTo(QDateTime::fromMSecsSinceEpoch(target));
    }

    static pa_time_event *newTimer(pa_mainloop_api *a, const struct timeval *tv, pa_time_event_cb_t callback, void *userdata)
    {
        QTimer *timer = new QTimer;
        timer->setProperty("PA_USERDATA", QVariant::fromValue(userdata));
        timer->setParent(qApp);
        timer->setSingleShot(true);

        Qt::TimerType timerType = Qt::VeryCoarseTimer;

        if (tv->tv_usec) {
            timerType = Qt::PreciseTimer;
        }

        timer->setTimerType(timerType);
        QObject::connect(timer, &QTimer::timeout, [ = ]() {
            callback(a, reinterpret_cast<pa_time_event *>(timer), tv, userdata);
        });
        int duration = msecsUntilTimeval(tv);

        if (duration < 0) {
            qWarning() << "Invalid timer target, sec:" << tv->tv_sec << "usec" << tv->tv_usec;
        }

        timer->start(duration);

        return reinterpret_cast<pa_time_event *>(timer);
    }

    static void restartTimer(pa_time_event *e, const timeval *tv)
    {
        QTimer *timer = reinterpret_cast<QTimer *>(e);
        Qt::TimerType timerType = Qt::VeryCoarseTimer;

        if (tv->tv_usec) {
            timerType = Qt::PreciseTimer;
        }

        timer->setTimerType(timerType);
        int duration = msecsUntilTimeval(tv);

        if (duration < 0) {
            qWarning() << "Invalid restart timer target, sec:" << tv->tv_sec << "usec" << tv->tv_usec;
        }

        timer->start(duration);
    }

    static void freeTimer(pa_time_event *e)
    {
        QTimer *timer = reinterpret_cast<QTimer *>(e);
        delete timer;
    }

    static void timerSetDestructor(pa_time_event *e, pa_time_event_destroy_cb_t destructor)
    {
        QTimer *timer = reinterpret_cast<QTimer *>(e);
        QObject::connect(timer, &QTimer::destroyed, [ = ]() {
            destructor(reinterpret_cast<pa_mainloop_api *>(timer->parent()), e, qvariant_cast<void *>(timer->property("PA_USERDATA")));
        });
    }

    struct SocketNotifierWrapper {
        SocketNotifierWrapper() = default;
        SocketNotifierWrapper(const SocketNotifierWrapper &) = delete;
        const SocketNotifierWrapper &operator=(const SocketNotifierWrapper &) = delete;
        ~SocketNotifierWrapper()
        {
            delete readNotifier;
            delete writeNotifier;
            delete errorNotifier;

            if (destructor) {
                destructor(a, reinterpret_cast<pa_io_event *>(this), userdata);
            }
        }

        QSocketNotifier *readNotifier = nullptr;
        QSocketNotifier *writeNotifier = nullptr;
        QSocketNotifier *errorNotifier = nullptr;

        pa_io_event_destroy_cb_t destructor = nullptr;

        void *userdata;
        pa_mainloop_api *a;
    };

    static pa_io_event *newIoEvent(pa_mainloop_api *a, int fd, pa_io_event_flags_t events, pa_io_event_cb_t cb, void *userdata)
    {
        SocketNotifierWrapper *wrapper = new SocketNotifierWrapper;
        wrapper->userdata = userdata;
        wrapper->a = a;

        wrapper->readNotifier = new QSocketNotifier(fd, QSocketNotifier::Read, qApp);
        QObject::connect(wrapper->readNotifier, &QSocketNotifier::activated, [ = ]() {
            cb(a, reinterpret_cast<pa_io_event *>(wrapper), fd, PA_IO_EVENT_INPUT, userdata);
        });

        wrapper->writeNotifier = new QSocketNotifier(fd, QSocketNotifier::Write, qApp);
        QObject::connect(wrapper->writeNotifier, &QSocketNotifier::activated, [ = ]() {
            cb(a, reinterpret_cast<pa_io_event *>(wrapper), fd, PA_IO_EVENT_OUTPUT, userdata);
        });

        wrapper->errorNotifier = new QSocketNotifier(fd, QSocketNotifier::Exception, qApp);

        QObject::connect(wrapper->errorNotifier, &QSocketNotifier::activated, [ = ]() {
            cb(a, reinterpret_cast<pa_io_event *>(wrapper), fd, PA_IO_EVENT_ERROR, userdata);
        });

        if (events & PA_IO_EVENT_INPUT) {
            wrapper->readNotifier->setEnabled(true);
        } else {
            wrapper->readNotifier->setEnabled(false);
        }


        if (events & PA_IO_EVENT_OUTPUT) {
            wrapper->writeNotifier->setEnabled(true);
        } else {
            wrapper->writeNotifier->setEnabled(false);
        }

        if (events & PA_IO_EVENT_ERROR || events & PA_IO_EVENT_HANGUP) {
            wrapper->errorNotifier->setEnabled(true);
        } else {
            wrapper->errorNotifier->setEnabled(false);
        }

        return reinterpret_cast<pa_io_event *>(wrapper);
    }

    static void setIoEnabled(pa_io_event *e, pa_io_event_flags_t events)
    {
        SocketNotifierWrapper *wrapper = reinterpret_cast<SocketNotifierWrapper *>(e);

        if (events & PA_IO_EVENT_INPUT) {
            wrapper->readNotifier->setEnabled(true);
        } else {
            wrapper->readNotifier->setEnabled(false);
        }

        if (events & PA_IO_EVENT_OUTPUT) {
            wrapper->writeNotifier->setEnabled(true);
        } else {
            wrapper->writeNotifier->setEnabled(false);
        }

        if (events & PA_IO_EVENT_ERROR || events & PA_IO_EVENT_HANGUP) {
            wrapper->errorNotifier->setEnabled(true);
        } else {
            wrapper->errorNotifier->setEnabled(false);
        }
    }

    static void ioDestroy(pa_io_event *e)
    {
        SocketNotifierWrapper *wrapper = reinterpret_cast<SocketNotifierWrapper *>(e);
        delete wrapper;
    }

    static void setIoDestructor(pa_io_event *e, pa_io_event_destroy_cb_t cb)
    {
        SocketNotifierWrapper *wrapper = reinterpret_cast<SocketNotifierWrapper *>(e);
        wrapper->destructor = cb;
    }

    static pa_defer_event *newDefer(pa_mainloop_api *a, pa_defer_event_cb_t callback, void *userdata)
    {
        QTimer *timer = new QTimer();
        timer->setProperty("PA_USERDATA", QVariant::fromValue(userdata));
        timer->setParent(qApp);
        //timer->setParent(reinterpret_cast<QtPaMainLoop*>(a));
        timer->setSingleShot(true);

        QObject::connect(timer, &QTimer::timeout, [ = ]() {
            callback(a, reinterpret_cast<pa_defer_event *>(timer), userdata);
        });
        timer->start(0);

        return reinterpret_cast<pa_defer_event *>(timer);
    }

    static void setDeferEnabled(pa_defer_event *e, int b)
    {
        QTimer *timer = reinterpret_cast<QTimer *>(e);

        if (b) {
            timer->start(0);
        } else {
            timer->stop();
        }
    }

    static void freeDefer(pa_defer_event *e)
    {
        QTimer *timer = reinterpret_cast<QTimer *>(e);
        delete timer;
    }

    static void deferSetDestructor(pa_defer_event *e, pa_defer_event_destroy_cb_t destructor)
    {
        QTimer *timer = reinterpret_cast<QTimer *>(e);
        QObject::connect(timer, &QTimer::destroyed, [ = ]() {
            destructor(reinterpret_cast<pa_mainloop_api *>(timer->parent()), e, qvariant_cast<void *>(timer->property("PA_USERDATA")));
        });
    }

    static void quit(pa_mainloop_api *a, int retval)
    {
        Q_UNUSED(a);

        qApp->exit(retval);
    }
};

