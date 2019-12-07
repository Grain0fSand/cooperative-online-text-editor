//
// Created by oem on 12/7/19.
//

#ifndef TRANSLATOR_REPLYTIMEOUT_H
#define TRANSLATOR_REPLYTIMEOUT_H


#include <QtCore/QTimerEvent>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QBasicTimer>
#include <iostream>

class ReplyTimeout : public QObject {
    Q_OBJECT
            QBasicTimer m_timer;
public:
    ReplyTimeout(QNetworkReply* reply, const int timeout) : QObject(reply) {
        Q_ASSERT(reply);
        if (reply && reply->isRunning())
            m_timer.start(timeout, this);
    }
    static void set(QNetworkReply* reply, const int timeout) {
        new ReplyTimeout(reply, timeout);
    }
protected:
    void timerEvent(QTimerEvent * ev) override {
        if (!m_timer.isActive() || ev->timerId() != m_timer.timerId())
            return;

        auto reply = static_cast<QNetworkReply*>(parent());
        if (reply->isRunning()){
            reply->close();
            reply->abort();

            // TODO: emit offline perchè ci ha messo troppo a rispondere
            std::cout << "too mutch time for the reply" << std::endl;
        }

        m_timer.stop();
    }
};


#endif //TRANSLATOR_REPLYTIMEOUT_H
