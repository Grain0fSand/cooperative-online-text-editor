#include <iostream>
#include <QEventLoop>
#include <forms/loginwindow.h>
#include "online_sender.h"

#define IP_ADDRESS "192.168.1.114"

OnlineSender::OnlineSender(std::string json_to_send,std::string docId,std::string token) :
    json_to_send(json_to_send),
    docId(docId),
    token(token)
{
    // because the background thread cannot communicate with the gui thread
    connect(this,&OnlineSender::prepareRequest,this,&OnlineSender::pushCrdtRequest);

    // the QTObj must be always be manipulated only by
    // the QThread that create the obj, so all the code must be
    // in the same thread: the run() method
    connect(&manager, &QNetworkAccessManager::finished, this, &OnlineSender::checkPushCrdtReply);
}

OnlineSender::OnlineSender(QString email, QString username, QString password, QString avatar) :
    email(email),
    username(username),
    password(password),
    avatar(avatar)
{
    moveToThread(this);
    connect(this,&OnlineSender::prepareRequest,this,&OnlineSender::tryRegistrationRequest);
    connect(&manager,&QNetworkAccessManager::finished,this,&OnlineSender::checkTryRegistrationReply);
    connect(this,&OnlineSender::responseTryRegistrationArrived,&LoginWindow::getInstance(),&LoginWindow::getRegisterResponse);
    connect(this,&OnlineSender::responseTryRegistrationArrived,this,&OnlineSender::quit,Qt::QueuedConnection);
}

OnlineSender::OnlineSender(QString username, QString password) :
    username(username),
    password(password)
{
    moveToThread(this);
    connect(this,&OnlineSender::prepareRequest,this,&OnlineSender::tryLoginRequest);
    connect(&manager,&QNetworkAccessManager::finished,this,&OnlineSender::checkTryLoginReply);
    connect(this,&OnlineSender::responseTryLoginArrived,this,&OnlineSender::quit,Qt::QueuedConnection);
}

void OnlineSender::run()
{
    emit prepareRequest();
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

void OnlineSender::checkPushCrdtReply(QNetworkReply *reply)
{
    if (reply->error()) {
        QString err = reply->errorString();
        qDebug() << err;
        return;
    }

    std::string answer = reply->readAll().toStdString();
    std::cout << answer;
    if (answer != "1")
        std::cout << "error: the server is not reachable";
    // TODO: remove comment here
    //emit responsePushCrdtArrived(answer);
}

void OnlineSender::checkTryRegistrationReply(QNetworkReply *reply)
{
    QString response_text;
    bool good_response = false;

    if (reply->error()) {
        qDebug() << reply->errorString();
        if(reply->error() == QNetworkReply::UnknownNetworkError)
            response_text = "Cannot connect to the server.\n"
                            "Check your Internet connection\n"
                            "and try again.";
        else if(reply->error() == QNetworkReply::ConnectionRefusedError || reply->error() == QNetworkReply::UnknownServerError)
            response_text = "Server not responding.\n"
                            "Please, try again later";
        else
            response_text = "Unknown connection error.\n"
                            "Please, contact the developers.";
    }
    else {
        int replyCode = reply->readAll().toInt();

        switch(replyCode) {
            case 0:
                response_text = "Account registered successfully!";
                good_response = true;
                break;
            case 1:
                response_text = "Email address already in use";
                break;
            case 2:
                response_text = "Username already in use\n"
                                "Please, choose another one";
                break;
            default:
                break;
        }
    }
    emit responseTryRegistrationArrived(good_response, response_text);
}

void OnlineSender::checkTryLoginReply(QNetworkReply *reply)
{
    if (reply->error()) {
        QString err = reply->errorString();
        std::cout << err.toStdString();
        return;
    }

    std::string answer = reply->readAll().toStdString();
    std::cout << answer;

    //emit responseTryLoginArrived(answer);
}