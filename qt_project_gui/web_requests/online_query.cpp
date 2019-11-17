#include "online_query.h"
#include <thread>

#define IP_ADDRESS "localhost"
#define PORT "6969"

OnlineQuery::OnlineQuery(std::string docId,std::string token,QObject* m) :
    docId(docId),
    token(token),
    lastCrdtId(""),
    continue_run(true)
{
    // because the background thread cannot communicate with the gui thread
    connect(this,&OnlineQuery::request_time,this,&OnlineQuery::getCrdtRequest);
    connect(this,&OnlineQuery::send_actions,&Crdt::getInstance(),&Crdt::update_income);
    connect(this,SIGNAL(update_id(std::string)),m,SLOT(update_id(std::string)));
    connect(this,SIGNAL(users_online_arrived(std::vector<exchangeable_data::user>)),
            m,SLOT(update_online_users_and_cursors_positions(std::vector<exchangeable_data::user>)));

    // the QTObj must be always be manipulated only by
    // the QThread that create the obj, so all the code must be
    // in the same thread: the run() method
    QObject::connect(&manager, &QNetworkAccessManager::finished, this, &OnlineQuery::checkReply);
}

void OnlineQuery::run() {
    // while loop
    while(continue_run) {
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
    manager.get(req);
}

void OnlineQuery::checkReply(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    std::string answer = reply->readAll().toStdString();
    json v = json::parse(answer);
    std::vector<exchangeable_data::send_data> array;

    int last = 0;

    if(lastCrdtId!="")
        last = std::stoi(lastCrdtId);

    int cmp;

    json j = v.at(0);
    json users = v.at(1);

    for (auto& element : j) {
        exchangeable_data::send_data data;
        exchangeable_data::send_data::from_json(data, element);
        cmp = std::stoi(data.id);

        if(cmp>last){
            array.push_back(data);
            lastCrdtId = data.id;
        }
    }

    std::vector<exchangeable_data::user> users_decoded;

    for (json& user : users){
        exchangeable_data::user u;
        exchangeable_data::user::from_json(u,user);

        users_decoded.push_back(u);
    }

    std::vector<ActionWrapper> actions;

    for(exchangeable_data::send_data& act : array){
        ActionWrapper w;
        ActionWrapper::action_from_json(w,json::parse(act.crdt));
        actions.push_back(w);
    }
    for (auto t : actions)
        if(t.action.getActionType() == Insertion)
            qDebug() << t.action.getChars().toUtf8();
        else qDebug() << t.action.getActionType();

    emit update_id(lastCrdtId);
    emit users_online_arrived(users_decoded);
    emit send_actions(actions);  //TODO pass reference
    // NO! you need to check cpp move Rvalue, the performance
    // of the copy is eigther superior than pass per reference
    // also note that here we are at tail of function that enable
    // the compiler to do also more magic

    // TODO: remove comment here
    //emit response_arrived(answer);
}
