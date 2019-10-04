#include <iostream>
#include <QEventLoop>
#include <forms/loginwindow.h>
#include "online_sender.h"
#include <QTimer>

#define IP_ADDRESS "192.168.1.114"
#define PORT "8081"

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
    connect(this,&OnlineSender::responseTryRegistrationArrived,&LoginWindow::getInstance(),&LoginWindow::showRegisterResponse);
    connect(this,&OnlineSender::responseTryRegistrationArrived,this,&OnlineSender::quit);
}

OnlineSender::OnlineSender(QString username, QString password) :
    username(username),
    password(password)
{
    moveToThread(this);
    connect(this,&OnlineSender::prepareRequest,this,&OnlineSender::tryLoginRequest);
    connect(&manager,&QNetworkAccessManager::finished,this,&OnlineSender::checkTryLoginReply);
    connect(this,&OnlineSender::responseTryLoginArrived,&LoginWindow::getInstance(),&LoginWindow::showLoginResponse);
    connect(this,&OnlineSender::responseTryLoginArrived,this,&OnlineSender::quit);
}

void OnlineSender::run()
{
    emit prepareRequest();
}

void OnlineSender::pushCrdtRequest()
{
    QString ip_address = IP_ADDRESS;
    QString port = PORT;
    QString location = "http://" + ip_address + ":" + PORT + "/";
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
    QString port = PORT;
    QString location = "http://" + ip_address + ":" + PORT + "/";
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
    QNetworkReply *reply = manager.get(req);

    QEventLoop loop(this);
    QTimer timeout;
    timeout.setSingleShot(true);
    connect(&timeout,&QTimer::timeout,&loop,&QEventLoop::quit);
    connect(&timeout,&QTimer::timeout,[&](){manager.finished(reply);});
    timeout.start(5000);
    loop.exec();
}

void OnlineSender::tryLoginRequest()
{
    QString ip_address = IP_ADDRESS;
    QString port = PORT;
    QString location = "http://" + ip_address + ":" + PORT + "/";
    QString request = "try_login";
    QString params = "?";
    params += "username=" + username;
    params += "&";
    params += "password=" + password;

    url.setUrl(location+request+params);
    req.setUrl(url);
    QNetworkReply *reply = manager.get(req);

    QEventLoop loop(this);
    QTimer timeout;

    timeout.setSingleShot(true);
    connect(&timeout,&QTimer::timeout,&loop,&QEventLoop::quit);
    connect(&timeout,&QTimer::timeout,[&](){manager.finished(reply);});
    timeout.start(3000);
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

    if(!reply->isFinished()) {
        reply->abort();
    }

    if (reply->error()) {
        qDebug() << reply->errorString();
        response_text = checkConnection(reply->error());
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
                response_text = "Username already in use";
                break;
            default:
                qDebug() << reply->errorString();
                break;
        }
    }
    emit responseTryRegistrationArrived(good_response, response_text);
}

void OnlineSender::checkTryLoginReply(QNetworkReply *reply)
{
    QString response_text;
    bool good_response = false;

    if(!reply->isFinished()) {
        reply->abort();
    }

    if (reply->error()) {
        qDebug() << reply->errorString();
        response_text = checkConnection(reply->error());
    }
    else {
        int replyCode = reply->readAll().toInt();
        switch(replyCode) {
            case 0:
                response_text = "Login failed!\n"
                                "Wrong username or password\n";
                break;
            case 1:
                response_text = "Login successful!";
                good_response = true;
                break;
            default:
                qDebug() << reply->errorString();
                break;
        }
    }
    emit responseTryLoginArrived(good_response, response_text);
}

QString OnlineSender::checkConnection(QNetworkReply::NetworkError error)
{
    if(error == QNetworkReply::UnknownNetworkError)
        return "Cannot connect to the server\n"
               "Check your Internet connection\n";
    else if(error == QNetworkReply::UnknownServerError)
        return "Cannot connect to the server\n"
               "Check your firewall settings\n";
    else if(error == QNetworkReply::OperationCanceledError || error == QNetworkReply::ConnectionRefusedError)
        return "Server not responding\n"
               "Please, try again later\n";
    else
        return "Unknown connection error\n"
               "Please, contact the developers.\n";
}