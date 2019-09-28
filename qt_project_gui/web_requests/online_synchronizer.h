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

using json = nlohmann::json;

class SynkObj {

public:

    SynkObj(std::string id,std::string action) : id_last_action(id),action_json(action) {}

    // NB: is a std::string because QString are constructable only throw the main thread
    // because each children must know the father, and that is possible only thread unsafe

    std::string id_last_action;
    std::string action_json; // temporary as like this! in the future the type is strongly typed and meaningfull
    // and has also methods for translating itself from json to obj and vice-versa
};

class OnlineQuery : public QThread
{
    Q_OBJECT

public:
    // TODO: if you want change lastcrdt, now it is setted as no previus crdt during construction
    // has to be replaced if you want restore the situation of the editor from a file
    OnlineQuery(std::string docId,std::string token) : token(token),docId(docId),lastcrdt("") {
        // because the background thread cannot communicate with the gui thread
        connect(this,&OnlineQuery::request_time,this,&OnlineQuery::request);




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

            for (auto& element : j) {
                exchangable_data::send_data data;
                exchangable_data::send_data::from_json(data, element);
                array.push_back(data);

                // TODO: update that and make it work
                //MainWindow::getInstance().sessionData.lastCrdtId = data.id;
            }

            std::vector<ActionWrapper> actions;

            for(exchangable_data::send_data act : array){
                ActionWrapper w;
                ActionWrapper::action_from_json(w,json::parse(act.crdt));
                actions.push_back(w);
            }

            std::cout << "ciao mondo";



            // TODO: remove comment here
            //emit response_arrived(answer);
        }
        );
    }

    void run(void) override {
        // while loop
        while(true){
            qDebug() << "request sygnal launched";
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
        params=params+QString::fromStdString(lastcrdt);
        params=params+"&docId=";
        params=params+QString::fromStdString(docId);
        url.setUrl(base+params);
        req.setUrl(url);
        manager.get(req);


        qDebug() << "request launched";

        return;

    }

private:
    QNetworkAccessManager manager{this};
    QUrl url;
    QNetworkRequest req;
    std::string docId;
    std::string token;
    std::string lastcrdt;

signals:
    void response_arrived(std::string response);
    void request_time();

    // periodic web request for filling the queue of SynkObj
};

#endif // ONLINE_SYNCHRONIZER_H

