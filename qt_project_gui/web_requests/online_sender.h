//
// Created by Elia Migliore on 27/09/2019.
//

#ifndef TRANSLATOR_ONLINE_SENDER_H
#define TRANSLATOR_ONLINE_SENDER_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <vector>
#include <QMutex>

class OnlineSender : public QThread
{
    Q_OBJECT

public:
    OnlineSender(std::string json_to_sent,std::string docId,std::string token) : json_to_send(json_to_sent),docId(docId),token(token) {
        // because the background thread cannot communicate with the gui thread
        connect(this,&OnlineSender::request_time,this,&OnlineSender::request);

        std::cout << json_to_sent << docId << token << "request called";



        // the QTObj must be always be manipulated only by
        // the QThread that create the obj, so all the code must be
        // in the same thread: the run() method
        QObject::connect(&manager, &QNetworkAccessManager::finished,
                         this, [=](QNetworkReply *reply) {

                    if (reply->error()) {
                        QString err = reply->errorString();
                        std::cout << err.toUtf8().constData();
                        return;
                    }


                    std::string answer = reply->readAll().toStdString();
                    if (answer.compare("1")!=0)
                        std::cout << "error: the server is not reachable";

                    // TODO: remove comment here
                    //emit response_arrived(answer);
                }
        );
    }

    void run(void) override {
        emit request_time();
    }



public slots:

    void request() {
        QString base="http://localhost:8080/push_crdt?";
        QString params="token=";
        params=params+QString::fromStdString(token);
        params=params+"&crdt=";
        params=params+QString::fromStdString(json_to_send);
        params=params+"&docId=";
        params=params+QString::fromStdString(docId);
        url.setUrl(base+params);
        req.setUrl(url);
        manager.get(req);

        std::cout << "signal launghed" << std::endl;

    }

 signals:
    void request_time();

private:
    std::string json_to_send;
    std::string token;
    std::string docId;
    QNetworkAccessManager manager{this};
    QUrl url;
    QNetworkRequest req;

    signals:
            void response_arrived(std::string response);
};

#endif //TRANSLATOR_ONLINE_SENDER_H
