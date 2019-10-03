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
    OnlineSender(QString email,QString username,QString password,QString encodedAvatar); //for registration
    OnlineSender(QString username,QString password); //for login

    void run() override;

public slots:
    void pushCrdtRequest();
    void tryRegistrationRequest();
    void tryLoginRequest();
    void checkPushCrdtReply(QNetworkReply *reply);
    void checkTryRegistrationReply(QNetworkReply *reply);
    void checkTryLoginReply(QNetworkReply *reply);

 signals:
    void prepareRequest();
    void responsePushCrdtArrived(std::string response);
    void responseTryRegistrationArrived(bool good_response, QString response_text);
    void responseTryLoginArrived(bool good_response, QString response_text);

private:
    std::string json_to_send;
    std::string docId;
    std::string token;
    QString email;
    QString username;
    QString password;
    QString avatar;

    QNetworkAccessManager manager{this};
    QUrl url;
    QNetworkRequest req;
};

#endif //ONLINE_SENDER_H
