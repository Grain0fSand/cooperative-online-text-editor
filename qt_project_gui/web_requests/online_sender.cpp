#include <iostream>
#include <QEventLoop>
#include <forms/loginwindow.h>
#include "online_sender.h"
#include <QTimer>

#define IP_ADDRESS "192.168.1.114"
#define PORT "6969"

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

OnlineSender::OnlineSender(std::string token,std::string filename) :
    token(token),
    filename(filename)
{
    moveToThread(this);
    connect(this,&OnlineSender::prepareRequest,this,&OnlineSender::newDocRequest);
    connect(&manager,&QNetworkAccessManager::finished,this,&OnlineSender::checkNewDocReply);
    connect(this,&OnlineSender::responseNewDocArrived,&LoginWindow::getInstance(),&LoginWindow::showNewDocResponse);
    connect(this,&OnlineSender::responseNewDocArrived,this,&OnlineSender::quit);
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

void OnlineSender::newDocRequest()
{
    QString ip_address = IP_ADDRESS;
    QString port = PORT;
    QString location = "http://" + ip_address + ":" + PORT + "/";
    QString request = "new_doc";
    QString params = "?";
    params += "token=" + QString::fromStdString(token);
    params += "&";
    params += "filename=" + QString::fromStdString(filename);

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
    QString responseText;
    bool goodResponse = false;

    if(!reply->isFinished()) {
        reply->abort();
    }

    if (reply->error()) {
        qDebug() << reply->errorString();
        responseText = checkConnection(reply->error());
    }
    else {
        int replyCode = reply->readAll().toInt();

        switch(replyCode) {
            case 0:
                responseText = "Account registered successfully!";
                goodResponse = true;
                break;
            case 1:
                responseText = "Email address already in use";
                break;
            case 2:
                responseText = "Username already in use";
                break;
            default:
                qDebug() << reply->errorString();
                break;
        }
    }
    emit responseTryRegistrationArrived(goodResponse, responseText);
}

void OnlineSender::checkTryLoginReply(QNetworkReply *reply)
{
    QString responseText;
    bool goodResponse = false;
    QString replyString;

    if(!reply->isFinished()) {
        reply->abort();
    }

    if (reply->error()) {
        qDebug() << reply->errorString();
        responseText = checkConnection(reply->error());
    }
    else {
        replyString = reply->readAll();
        int replyCode = replyString.left(1).toInt();

        switch(replyCode) {
            case 0:
                responseText = "Login failed!\n"
                               "Wrong username or password\n";
                break;
            case 1:
                responseText = "Login successful!";
                goodResponse = true;
                replyString.remove(0,2);
                break;
            default:
                qDebug() << reply->errorString();
                break;
        }
    }
    emit responseTryLoginArrived(goodResponse, responseText, replyString);
}

void OnlineSender::checkNewDocReply(QNetworkReply *reply)
{
    QString responseText;
    bool goodResponse = false;
    QStringList replyParts;

    if(!reply->isFinished()) {
        reply->abort();
    }

    if (reply->error()) {
        qDebug() << reply->errorString();
        responseText = checkConnection(reply->error());
    }
    else {
        replyParts = QString(reply->readAll()).split("|");

        switch(replyParts[0].toInt()) {
            case 0:
                responseText = "A document with this name already exist!\n";
                break;
            case 1:
                responseText = "New document created on server!";
                MainWindow::getInstance().sessionData.docId = replyParts[1].toStdString();
                MainWindow::getInstance().sessionData.status = true;
                goodResponse = true;
                break;
            default:
                qDebug() << reply->errorString();
                break;
        }
    }
    emit responseNewDocArrived(goodResponse, responseText);
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