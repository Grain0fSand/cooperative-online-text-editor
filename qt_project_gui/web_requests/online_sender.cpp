#include <iostream>
#include "online_sender.h"

#define IP_ADDRESS "localhost"

OnlineSender::OnlineSender(std::string json_to_send,std::string docId,std::string token) :
    json_to_send(json_to_send),
    docId(docId),
    token(token)
{
    // because the background thread cannot communicate with the gui thread
    connect(this,&OnlineSender::request_time,this,&OnlineSender::pushCrdtRequest);

    // the QTObj must be always be manipulated only by
    // the QThread that create the obj, so all the code must be
    // in the same thread: the run() method
    connect(&manager, &QNetworkAccessManager::finished, this, &OnlineSender::checkReply);
}

OnlineSender::OnlineSender(std::string email,std::string username,std::string password, int i) :
    email(email),
    username(username),
    password(password)
{
    connect(this,&OnlineSender::request_time,this,&OnlineSender::tryRegistrationRequest);
    connect(&manager, &QNetworkAccessManager::finished, this, &OnlineSender::checkReply);
}

OnlineSender::OnlineSender(std::string username,std::string password) :
        username(username),
        password(password)
{
    connect(this,&OnlineSender::request_time,this,&OnlineSender::tryLoginRequest);
    connect(&manager, &QNetworkAccessManager::finished, this, &OnlineSender::checkReply);
}

void OnlineSender::run()
{
    emit request_time();
}

void OnlineSender::checkReply(QNetworkReply *reply)
{
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

void OnlineSender::pushCrdtRequest()
{
    QString ip_address = IP_ADDRESS;
    QString location = "http://" + ip_address + ":8080/";
    QString request = "push_crdt";
    QString params = "?";
    params += "token=" + QString::fromStdString(token);
    params += "&";
    params += "crdt=" + QString::fromStdString(json_to_send);
    params += "&";
    params += "docId=" + QString::fromStdString(docId);

    url.setUrl(location+request+params);
    req.setUrl(url);
    manager.get(req);
}

void OnlineSender::tryRegistrationRequest()
{
    QString ip_address = IP_ADDRESS;
    QString location = "http://" + ip_address + ":8080/";
    QString request = "try_registration";
    QString params = "?";
    params += "email=" + QString::fromStdString(email);
    params += "&";
    params += "username=" + QString::fromStdString(username);
    params += "&";
    params += "password=" + QString::fromStdString(password);

    url.setUrl(location+request+params);
    req.setUrl(url);
    manager.get(req);
}

void OnlineSender::tryLoginRequest()
{
    QString ip_address = IP_ADDRESS;
    QString location = "http://" + ip_address + ":8080/";
    QString request = "try_login";
    QString params = "?";
    params += "username=" + QString::fromStdString(username);
    params += "&";
    params += "password=" + QString::fromStdString(password);

    url.setUrl(location+request+params);
    req.setUrl(url);
    manager.get(req);
}