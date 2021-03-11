//
// Created by oem on 12/7/19.
//

#ifndef TRANSLATOR_REPLYTIMEOUT_H
#define TRANSLATOR_REPLYTIMEOUT_H


#include <QtCore/QTimerEvent>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QBasicTimer>
#include <iostream>
#include <forms/mainwindow.h>

class ReplyTimeout : public QObject {
    Q_OBJECT
            QBasicTimer m_timer;
public:
    ReplyTimeout(QNetworkReply* reply, const int timeout) : QObject(reply) {
        Q_ASSERT(reply);
        if (reply && reply->isRunning()) {
            connect(this,&ReplyTimeout::timeoutOccurred,SessionData::accessToSessionData().mainWindowPointer,&MainWindow::changeEditorStatus);
            m_timer.start(timeout, this);
        }
    }
    static void set(QNetworkReply* reply, const int timeout) {
        new ReplyTimeout(reply, timeout);
    }

signals:
    void timeoutOccurred();

protected:
    void timerEvent(QTimerEvent * ev) override {
        if (!m_timer.isActive() || ev->timerId() != m_timer.timerId())
            return;

        auto reply = static_cast<QNetworkReply*>(parent());
        if (reply->isRunning()){
            reply->close();
            reply->abort();

            SessionData::accessToSessionData().mutex_online.lock();
            bool tmp = SessionData::accessToSessionData().isUserOnline;
            SessionData::accessToSessionData().mutex_online.unlock();
            if(tmp)
                emit timeoutOccurred();

           // std::cout << "Too much time for the reply" << std::endl;
        }

        m_timer.stop();
    }
};


#endif //TRANSLATOR_REPLYTIMEOUT_H
