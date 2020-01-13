#include "online_query.h"
#include "../data_structure/session_data.h"
#include "ReplyTimeout.h"
#include <thread>
#include <QtCore/QBuffer>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QProcess>
#include <QtCore/QBasicTimer>
#include <QTimer>

#define IP_ADDRESS "47.53.242.167"
//#define IP_ADDRESS "192.168.1.114"
#define PORT "6969"

OnlineQuery::OnlineQuery(std::string docId,std::string token,QObject* m) :
    docId(docId),
    token(token),
    lastCrdtId("")
{
    // because the background thread cannot communicate with the gui thread
    connect(this,&OnlineQuery::request_time,this,&OnlineQuery::getCrdtRequest);
    connect(this,&OnlineQuery::send_actions,&Crdt::getInstance(),&Crdt::update_income);
    connect(this,&OnlineQuery::users_online_arrived,SessionData::accessToSessionData().mainWindowPointer,&MainWindow::arrangeUserTagList);
    connect(this,&OnlineQuery::user_changed_his_status,SessionData::accessToSessionData().mainWindowPointer,&MainWindow::changeEditorStatus);
    connect(SessionData::accessToSessionData().mainWindowPointer,&MainWindow::userGoneOffline,[&](){lastCrdtId="";});
    connect(SessionData::accessToSessionData().mainWindowPointer,&MainWindow::stopQueryLoop,this,&OnlineQuery::terminate);
    // the QTObj must be always be manipulated only by
    // the QThread that create the obj, so all the code must be
    // in the same thread: the run() method
    QObject::connect(&manager, &QNetworkAccessManager::finished, this, &OnlineQuery::checkReply);
}

void OnlineQuery::run() {
    // while loop
    while(true) {
        emit request_time();
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }
}

void OnlineQuery::getCrdtRequest() {
    // body of webrequest and json decode/unmarshaling

    QString ip_address = IP_ADDRESS;
    QString port = PORT;
    QString location = "http://" + ip_address + ":" + PORT + "/";
    QString request = "get_crdt";
    QString params = "?";
    params += "token=" + QString::fromStdString(token);
    params += "&";
    params += "lastcrdt=" + QString::fromStdString(lastCrdtId);
    params += "&";
    params += "docId=" + QString::fromStdString(docId);
    params += "&";
    params += "remoteCursor="; //TODO: add the crdt symbol relative to cursor position

    url.setUrl(location+request+params);
    req.setUrl(url);
    QNetworkReply* reply = manager.get(req);
    ReplyTimeout::set(reply,750);
    connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(slotErrorConnection()));
}

void OnlineQuery::slotErrorConnection() {
    if (SessionData::accessToSessionData().isUserOnline)
        emit user_changed_his_status();
}

void OnlineQuery::checkReply(QNetworkReply *reply) {


    bool is_online = SessionData::accessToSessionData().isUserOnline;

    if (is_online && reply->error() == QNetworkReply::NoError && reply->bytesAvailable()){
       // std::cout << "sei online" << std::endl;
    } else {

        if (reply->error() == QNetworkReply::NoError && reply->bytesAvailable()){
            emit user_changed_his_status();
            std::cout << "sei tornato online" << std::endl;

        } else if (is_online){
            emit user_changed_his_status();
            std::cout << "ti sei appena scollegato" << std::endl;
        }

        return;
    }

    //TODO: this might never be true (see the first if-else above...)
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    std::string answer = reply->readAll().toStdString();
    json crdt_onlineUsers = json::parse(answer);
    json j = crdt_onlineUsers[0];
    json onlineUsers = crdt_onlineUsers[1];

    std::vector<exchangeable_data::send_data> array;
    std::vector<exchangeable_data::user> arrayOnlineUser;

    int last = 0;

    if(lastCrdtId!="")
        last = std::stoi(lastCrdtId);

    int cmp;

    for (auto& element : onlineUsers) {
        exchangeable_data::user user;
        exchangeable_data::user::from_json(user, element);
        arrayOnlineUser.push_back(user);
    }

    emit users_online_arrived(arrayOnlineUser);

    for (auto& element : j) {
        exchangeable_data::send_data data;
        exchangeable_data::send_data::from_json(data, element);
        cmp = std::stoi(data.id);

        if(cmp>last){
            array.push_back(data);
            lastCrdtId = data.id;
        }
    }


    std::vector<ActionWrapper> actions;

    for(exchangeable_data::send_data act : array){
        ActionWrapper w;
        std::string json_str = act.crdt;
        QString qjson_str = QString::fromStdString(json_str);

        QString decodedCrdt = QByteArray::fromBase64(qjson_str.toUtf8(),QByteArray::Base64UrlEncoding);

        ActionWrapper::action_from_json(w,json::parse(decodedCrdt.toStdString()));
        actions.push_back(w);
    }

    for (auto t : actions)
        if(t.action.getActionType() == Insertion)
            qDebug() << t.action.getChars().toUtf8();
        else qDebug() << t.action.getActionType();

    if (lastCrdtId!="")
        SessionData::accessToSessionData().lastCrdtId = lastCrdtId;

    emit send_actions(actions);

//    SessionData::accessToSessionData().myTextEditPointer->refreshCursors();
    //emit response_arrived(answer);
}

