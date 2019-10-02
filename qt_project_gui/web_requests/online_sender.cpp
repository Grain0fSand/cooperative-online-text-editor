#include <iostream>
#include <QEventLoop>
#include "online_sender.h"

#define IP_ADDRESS "192.168.1.114"

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

OnlineSender::OnlineSender(QString email, QString username, QString password, QString avatar) :
    email(email),
    username(username),
    password(password),
    avatar(avatar)
{
    moveToThread(this);
    connect(this,&OnlineSender::request_time,this,&OnlineSender::tryRegistrationRequest);
    connect(&manager, &QNetworkAccessManager::finished, this, &OnlineSender::checkReply);
    connect(this, &OnlineSender::response_arrived, this, &OnlineSender::quit);
}

OnlineSender::OnlineSender(QString username, QString password) :
        username(username),
        password(password)
{
    moveToThread(this);
    connect(this,&OnlineSender::request_time,this,&OnlineSender::tryLoginRequest);
    connect(&manager, &QNetworkAccessManager::finished, this, &OnlineSender::checkReply);
    connect(this, &OnlineSender::response_arrived, this, &OnlineSender::quit);
}

void OnlineSender::run()
{
    emit request_time();
}

void OnlineSender::checkReply(QNetworkReply *reply)
{
    if (reply->error()) {
        QString err = reply->errorString();
        std::cout << err.toStdString();
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
    params += "email=" + email;
    params += "&";
    params += "username=" + username;
    params += "&";
    params += "password=" + password;
    params += "&";
    params += "image=" + avatar;

    url.setUrl(location+request+params);
    req.setUrl(url);
    manager.get(req);

    QEventLoop loop(this);
    loop.exec();
}

void OnlineSender::tryLoginRequest()
{
    QString ip_address = IP_ADDRESS;
    QString location = "http://" + ip_address + ":8080/";
    QString request = "try_login";
    QString params = "?";
    params += "username=" + username;
    params += "&";
    params += "password=" + password;

    url.setUrl(location+request+params);
    req.setUrl(url);
    manager.get(req);

    QEventLoop loop(this);
    loop.exec();
}