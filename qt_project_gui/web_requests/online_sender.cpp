#include <iostream>
#include "online_sender.h"

OnlineSender::OnlineSender(std::string json_to_send,std::string docId,std::string token) :
    json_to_send(json_to_send),
    docId(docId),
    token(token)
{
    // because the background thread cannot communicate with the gui thread
    connect(this,&OnlineSender::request_time,this,&OnlineSender::request);

    //std::cout << json_to_send << docId << token << "request called";



    // the QTObj must be always be manipulated only by
    // the QThread that create the obj, so all the code must be
    // in the same thread: the run() method
    connect(&manager, &QNetworkAccessManager::finished,
            this, [=](QNetworkReply *reply) {

                if (reply->error()) {
                    QString err = reply->errorString();
                    std::cout << err.toUtf8().constData();
                    return;
                }


                std::string answer = reply->readAll().toStdString();
                if (answer != "1")
                    std::cout << "error: the server is not reachable";
                // TODO: remove comment here
                //emit response_arrived(answer);
            }
    );
}

void OnlineSender::run()
{
    emit request_time();
}

void OnlineSender::request()
{
    QString base = "http://192.168.1.114:8080/push_crdt?";
    QString params = "";
    params += "token=" + QString::fromStdString(token);
    params += "&";
    params += "crdt=" + QString::fromStdString(json_to_send);
    params += "&";
    params += "docId=" + QString::fromStdString(docId);
    qDebug() << params;
    url.setUrl(base+params);
    req.setUrl(url);
    manager.get(req);
}