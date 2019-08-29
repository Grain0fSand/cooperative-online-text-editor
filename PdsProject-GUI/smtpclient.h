#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QObject>
#include <QSslSocket>

class SmtpClient : public QObject
{
    Q_OBJECT
public:

    enum SmtpError
    {
        ConnectionTimeoutError,
        ResponseTimeoutError,
        SendDataTimeoutError,
        AuthenticationFailedError,
        ServerError,
        ClientError
    };

    SmtpClient();
    ~SmtpClient();

    void setHost(const QString &host);
    void setPort(uint16_t port);
    void setUser(const QString &user);
    void setPassword(const QString &password);

    bool connectToHost();
    void waitForResponse();
    bool login();
    void sendMessage(const QString &text);
    void quit();

    void setMailSender(QString address, QString name);
    void setMailDestination(QString address, QString name);
    void setMailSubject(QString subject);
    void prepareMailText(QString uri);
    bool sendMail();

signals:
    void smtpError(SmtpClient::SmtpError e);

public slots:

    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketError(QAbstractSocket::SocketError error);
    void socketReadyRead();

private:
    QSslSocket *socket;

    QString host;
    uint16_t port;
    QString name;

    QString user;
    QString password;

    int connectionTimeout;
    int responseTimeout;
    int sendMessageTimeout;

    QString responseText;
    int responseCode;

    QString emailSenderName;
    QString emailSenderAddress;
    QString emailDestName;
    QString emailDestAddress;
    QString emailSubject;
    QString emailText;
    QString toString();

    class ResponseTimeoutException {};
    class SendMessageTimeoutException {};
};

#endif // SMTPCLIENT_H
