#include "smtpclient.h"
#include <QMessageBox>

SmtpClient::SmtpClient() :
    socket(nullptr),
    name("localhost"),
    connectionTimeout(5000),
    responseTimeout(5000),
    sendMessageTimeout(60000)
{
    this->socket = new QSslSocket(this);
    this->host = "smtp.gmail.com";
    this->port = uint16_t(465);

    connect(socket,&QSslSocket::stateChanged,this,&SmtpClient::socketStateChanged);
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket,&QSslSocket::readyRead,this,&SmtpClient::socketReadyRead);
}

SmtpClient::~SmtpClient() {
    delete socket;
}

bool SmtpClient::connectToHost() {
    this->socket->connectToHostEncrypted(this->host,this->port);

    if (!this->socket->waitForConnected(connectionTimeout))
    {
        emit smtpError(ConnectionTimeoutError);
        return false;
    }
    try
    {
        waitForResponse();

        // If the response code is not 220 (Service ready)
        // means that is something wrong with the server
        if (responseCode != 220)
        {
            emit smtpError(ServerError);
            return false;
        }

        // The client's first command must be EHLO/HELO
        sendMessage("EHLO " + name);

        waitForResponse();

        // The response code needs to be 250.
        if (responseCode != 250) {
            emit smtpError(ServerError);
            return false;
        }
    }
    catch (ResponseTimeoutException)
    {
        return false;
    }
    catch (SendMessageTimeoutException)
    {
        return false;
    }

    return true;
}

bool SmtpClient::login()
{
    try {
        // Sending command: AUTH PLAIN base64('\0' + username + '\0' + password)
        sendMessage("AUTH PLAIN " + QByteArray().append('\0').append(this->user).append('\0').append(this->password).toBase64());

        waitForResponse();

        // If the response is not 235 then the authentication was failed
        if (responseCode != 235)
        {
            emit smtpError(AuthenticationFailedError);
            return false;
        }
    }
    catch (ResponseTimeoutException)
    {
        emit smtpError(AuthenticationFailedError);
        return false;
    }
    catch (SendMessageTimeoutException)
    {
        emit smtpError(AuthenticationFailedError);
        return false;
    }

    return true;
}

void SmtpClient::waitForResponse()
{
    do {
        if (!this->socket->waitForReadyRead(responseTimeout)) {
            emit smtpError(ResponseTimeoutError);
            throw ResponseTimeoutException();
        }
        while (this->socket->canReadLine()) {
            // Save the server's response
            this->responseText = socket->readLine();

            // Extract the respose code from the server's responce (first 3 digits)
            this->responseCode = responseText.left(3).toInt();

            if (responseCode / 100 == 4)
                emit smtpError(ServerError);

            if (responseCode / 100 == 5)
                emit smtpError(ClientError);

            if (responseText[3] == ' ') return;
        }
    } while (true);
}

void SmtpClient::sendMessage(const QString &text)
{
    this->socket->write(text.toUtf8() + "\r\n");
    if (!this->socket->waitForBytesWritten(sendMessageTimeout))
    {
      emit smtpError(SendDataTimeoutError);
      throw SendMessageTimeoutException();
    }
}

bool SmtpClient::sendMail()
{
    try
    {
        // Send the MAIL command with the sender
        sendMessage("MAIL FROM:<" + this->user + ">");

        waitForResponse();

        if (responseCode != 250) return false;

        sendMessage("RCPT TO:<" + this->emailDestAddress + ">");
        waitForResponse();

        if (responseCode != 250) return false;

        // Send DATA command
        sendMessage("DATA");
        waitForResponse();

        if (responseCode != 354) return false;

        sendMessage(this->toString());

        // Send \r\n.\r\n to end the mail data
        sendMessage(".");

        waitForResponse();

        if (responseCode != 250) return false;
    }
    catch (ResponseTimeoutException)
    {
        return false;
    }
    catch (SendMessageTimeoutException)
    {
        return false;
    }

    return true;
}

void SmtpClient::quit()
{
    try
    {
        sendMessage("QUIT");
    }
    catch(SmtpClient::SendMessageTimeoutException)
    {
    //Manually close the connection to the smtp server if message "QUIT" wasn't received by the smtp server
        if(socket->state() == QAbstractSocket::ConnectedState ||
           socket->state() == QAbstractSocket::ConnectingState ||
           socket->state() == QAbstractSocket::HostLookupState)
        {
            socket->disconnectFromHost();
        }
    }
}

void SmtpClient::setMailSender(QString address, QString name)
{
    this->emailSenderAddress = address;
    this->emailSenderName = name;
}

void SmtpClient::setMailDestination(QString address, QString name)
{
    this->emailDestAddress = address;
    this->emailDestName = name;
}

void SmtpClient::setMailSubject(QString subject)
{
    this->emailSubject = subject;
}

void SmtpClient::prepareMailText(QString uri)
{
    this->emailText = "Hey you!\n\n"

                      "You have been invited to collaborate in editing a document on SimulPad!\n"
                      "In order to participate in the project insert the following URI in the appropriate field of the initial menu:\n\n"

                      ""+uri+"\n\n"

                      "Best regards\n"
                      "Simulpad Text Editor";
}

void SmtpClient::setHost(const QString &host)
{
    this->host = host;
}

void SmtpClient::setPort(uint16_t port)
{
    this->port = port;
}

void SmtpClient::setUser(const QString &user)
{
    this->user = user;
}

void SmtpClient::setPassword(const QString &password)
{
    this->password = password;
}

QString SmtpClient::toString()
{
    QString mime;

    mime = "From:";
    mime += " " + this->emailSenderName;
    mime += " <" + this->emailSenderAddress + ">\r\n";
    mime += "To:";
    mime += " " + this->emailDestName;
    mime += " <" + this->emailDestAddress + ">";
    mime += "\r\n";
    mime += "Subject: ";
    mime += this->emailSubject;
    mime += "\r\n";
    mime += "MIME-Version: 1.0\r\n";
    mime += QString("Date: %1\r\n").arg(QDateTime::currentDateTime().toString(Qt::RFC2822Date));
    mime += this->emailText;

    return mime;
}

void SmtpClient::socketStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << state;
}

void SmtpClient::socketError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
    if(error==QAbstractSocket::HostNotFoundError || error==QAbstractSocket::SocketTimeoutError) {
        QMessageBox::critical(nullptr,"Error","Impossible to send the invitation in this moment.\nPlease check you Internet connection and try again");
    }
}

void SmtpClient::socketReadyRead()
{
}
