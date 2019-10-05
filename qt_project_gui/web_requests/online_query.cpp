#include "online_query.h"
#include <thread>

#define IP_ADDRESS "localhost"
#define PORT "6969"

OnlineQuery::OnlineQuery(std::string docId,std::string token,QObject* m) :
    docId(docId),
    token(token),
    lastCrdtId("")
{
    // because the background thread cannot communicate with the gui thread
    connect(this,&OnlineQuery::request_time,this,&OnlineQuery::getCrdtRequest);
    connect(this,&OnlineQuery::send_actions,&Crdt::getInstance(),&Crdt::update_income);
    connect(this,SIGNAL(update_id(std::string)),m,SLOT(update_id(std::string)));

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
    json j = json::parse(answer);
    std::vector<exchangeable_data::send_data> array;

    int last = 0;

    if(lastCrdtId!="")
        last = std::stoi(lastCrdtId);

    int cmp;

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
        ActionWrapper::action_from_json(w,json::parse(act.crdt));
        actions.push_back(w);
    }


    emit update_id(lastCrdtId);
    emit send_actions(actions);  //TODO pass reference

    // TODO: remove comment here
    //emit response_arrived(answer);
}
