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
    OnlineSender(std::string json_to_sent,std::string docId,std::string token){
        // because the background thread cannot communicate with the gui thread
        connect(this,&OnlineSender::request_time,this,&OnlineSender::request);




        // the QTObj must be always be manipulated only by
        // the QThread that create the obj, so all the code must be
        // in the same thread: the run() method
        QObject::connect(&manager, &QNetworkAccessManager::finished,
                         this, [=](QNetworkReply *reply) {

                    if (reply->error()) {
                        qDebug() << reply->errorString();
                        return;
                    }


                    std::string answer = reply->readAll().toStdString();
                    qDebug() << "request correctly terminated numchar: " << answer.size() ;

                    // TODO: remove comment here
                    //emit response_arrived(answer);
                }
        );
    }

    void run(void) override {
        // while loop
        while(true){
            qDebug() << "request sygnal launched";
            emit request_time();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }



public slots:

    void request() {
        url.setUrl("https://www.google.it/");
        req.setUrl(url);
        manager.get(req);


        qDebug() << "request launched";

    }

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
