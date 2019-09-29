#ifndef ONLINE_SYNCHRONIZER_H
#define ONLINE_SYNCHRONIZER_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <vector>
#include <QMutex>
#include <nlohmann/json.hpp>
#include "../data_structure/action.h"
#include "../forms/mainwindow.h"
#include "../exchangeable.h"
#include "../data_structure/crdt.h"

using json = nlohmann::json;

class OnlineQuery : public QThread
{
    Q_OBJECT

public:
    OnlineQuery(std::string docId,std::string token,QObject* m) : token(token),docId(docId),lastCrdtId("") {
        // because the background thread cannot communicate with the gui thread
        connect(this,&OnlineQuery::request_time,this,&OnlineQuery::request);
        connect(this,SIGNAL(send_actions(std::vector<ActionWrapper>)),
                &Crdt::getInstance(),SLOT(update_income(std::vector<ActionWrapper>)));
        connect(this,SIGNAL(update_id(std::string)),m,SLOT(update_id(std::string)));




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
            json j = json::parse(answer);
            std::vector<exchangable_data::send_data> array;



            int last = 0;

            if(lastCrdtId.compare("")!=0)
                last = std::stoi(lastCrdtId);

            int cmp;

            for (auto& element : j) {
                exchangable_data::send_data data;
                exchangable_data::send_data::from_json(data, element);
                cmp = std::stoi(data.id);

                if(cmp>last){
                    array.push_back(data);
                    lastCrdtId = data.id;
                }

            }

            std::vector<ActionWrapper> actions;

            for(exchangable_data::send_data act : array){
                ActionWrapper w;
                ActionWrapper::action_from_json(w,json::parse(act.crdt));
                actions.push_back(w);
            }


            emit update_id(lastCrdtId);
            emit send_actions(actions);

            // TODO: remove comment here
            //emit response_arrived(answer);
        }
        );
    }

    void run(void) override {
        // while loop
        while(true){
            emit request_time();
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
        }
    }



public slots:

    void request() {

        // body of webrequest and json decode/unmarshaling

        QString base="http://localhost:8080/get_crdt?";
        QString params="token=";
        params=params+QString::fromStdString(token);
        params=params+"&lastcrdt=";
        params=params+QString::fromStdString(lastCrdtId);
        params=params+"&docId=";
        params=params+QString::fromStdString(docId);
        url.setUrl(base+params);
        req.setUrl(url);
        manager.get(req);

        return;

    }

private:
    QNetworkAccessManager manager{this};
    QUrl url;
    QNetworkRequest req;
    std::string docId;
    std::string token;
    std::string lastCrdtId;

signals:
    void response_arrived(std::string response);
    void update_id(std::string id);
    void request_time();
    void send_actions(std::vector<ActionWrapper> actions);

    // periodic web request for filling the queue of SynkObj
};

#endif // ONLINE_SYNCHRONIZER_H

