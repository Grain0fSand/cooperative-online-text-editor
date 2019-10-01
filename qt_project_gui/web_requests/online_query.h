#ifndef ONLINE_QUERY_H
#define ONLINE_QUERY_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <vector>
#include <QMutex>
#include <nlohmann/json.hpp>
#include "../data_structure/action.h"
#include "../data_structure/exchangeable.h"
#include "../data_structure/crdt.h"

using json = nlohmann::json;

class OnlineQuery : public QThread
{
    Q_OBJECT

public:
    OnlineQuery(std::string docId,std::string token,QObject* m);

    void run() override;

public slots:
    void getCrdtRequest();
    void checkReply(QNetworkReply *reply);

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

#endif // ONLINE_QUERY_H