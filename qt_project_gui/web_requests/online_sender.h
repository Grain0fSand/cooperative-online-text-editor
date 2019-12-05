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
    //TODO: refactor the way to distinguish the requests
    OnlineSender(std::string json_to_send,std::string docId,std::string token); //for crdt push
    OnlineSender(QString email,QString username,QString password,QString encodedAvatar); //for registration
    OnlineSender(QString username,QString password); //for login
    OnlineSender(QString token, std::string docId); //for logout
    OnlineSender(std::string token,std::string newDocName); //for new doc creation
    OnlineSender(std::string token,QString existingDocName); //for open an existing doc
    OnlineSender(std::string token, QString username, QString encodedAvatar, QString password); //for updating user data

    void run() override;
    static QString checkConnection(QNetworkReply::NetworkError error);

public slots:
    void pushCrdtRequest();
    void tryRegistrationRequest();
    void tryLoginRequest();
    void logoutRequest();
    void newDocRequest();
    void getPartecipantsRequest();
    void updateUserDataRequest();
    void checkPushCrdtReply(QNetworkReply *reply);
    void checkTryRegistrationReply(QNetworkReply *reply);
    void checkTryLoginReply(QNetworkReply *reply);
    void checkNewDocReply(QNetworkReply *reply);
    void checkGetPartecipantsReply(QNetworkReply *reply);
    void checkUpdateUserDataReply(QNetworkReply *reply);

signals:
    void prepareRequest();
    void responsePushCrdtArrived(std::string response);
    void responseTryRegistrationArrived(bool goodResponse, QString responseText);
    void responseTryLoginArrived(bool goodResponse, QString responseText, QString replyString);
    void responseNewDocArrived(bool goodResponse, QString responseText, QString replyString);
    void responseGetPartecipantsArrived(QString replyString);
    void responseUpdateUserDataArrived(bool goodResponse, QString responseText);

private:
    std::string json_to_send;
    std::string docId;
    std::string token;
    std::string docName;
    QString email;
    QString username;
    QString password;
    QString avatar;

    QNetworkAccessManager manager{this};
    QUrl url;
    QNetworkRequest req;
};

#endif //ONLINE_SENDER_H
