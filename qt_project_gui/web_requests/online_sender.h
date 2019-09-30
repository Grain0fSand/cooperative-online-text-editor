#ifndef ONLINE_SENDER_H
#define ONLINE_SENDER_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <vector>
#include <QMutex>

class OnlineSender : public QThread
{
    Q_OBJECT

public:
    OnlineSender(std::string json_to_send,std::string docId,std::string token); //for crdt push

    void run() override;

public slots:
    void request();

 signals:
    void request_time();
    void response_arrived(std::string response);

private:
    std::string json_to_send;
    std::string docId;
    std::string token;

    QNetworkAccessManager manager{this};
    QUrl url;
    QNetworkRequest req;
};

#endif //ONLINE_SENDER_H
