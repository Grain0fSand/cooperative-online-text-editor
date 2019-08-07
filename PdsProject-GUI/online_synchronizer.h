#ifndef ONLINE_SYNCHRONIZER_H
#define ONLINE_SYNCHRONIZER_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <vector>
#include <QMutex>

class SynkObj {

public:

    SynkObj(std::string str) : str(str) {}

    // NB: is a std::string because QString are constructable only throw the main thread
    // because each children must know the father, and that is possible only thread unsafe

    std::string str; // temporary as like this! in the future the type is strongly typed and meaningfull
    // and has also methods for translating itself from json to obj and vice-versa
};

class OnlineQuery : public QThread
{
    Q_OBJECT

public:
    OnlineQuery(){
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
            qDebug() << "request correctly terminated numchar: " << answer.size() ;

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
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }



public slots:

    void request() {

        // body of webrequest and json decode/unmarshaling

        url.setUrl("https://www.google.it/");
        req.setUrl(url);
        manager.get(req);


        qDebug() << "request launched";

    }

private:
    QNetworkAccessManager manager{this};
    QUrl url;
    QNetworkRequest req;

signals:
    void response_arrived(std::string response);
    void request_time();

    // periodic web request for filling the queue of SynkObj
};

// for templating class is not possible split
// definition from implementation due to the
// size that the compiler must know for the type
// during the precompilation phases

class OnlineSynchronizer : public QObject
{
    Q_OBJECT

public:
    OnlineSynchronizer(){
        this->query.start();
        connect(&(this->query),&OnlineQuery::response_arrived,this,&OnlineSynchronizer::receiveSynkObj);
    }

signals:
    void sendSynkObj(std::string obj);

public slots:
    void receiveSynkObj(std::string obj){
        // for thread safe implementation
        QMutexLocker l(&synk_mutex_send);

        qDebug() << QString(obj.c_str());

        send.push_back(SynkObj(obj));

    }

private:
    QMutex synk_mutex_send;
    QMutex synk_mutex_receive;
    std::vector<SynkObj> send;
    std::vector<SynkObj> receive;
    OnlineQuery query; // obj that fill the queue due to QThread and QWebRequest and json decoder


    void sendSynkObjFromQueue(){
        // for thread safe implementation
        QMutexLocker l(&synk_mutex_receive);
        SynkObj obj = receive.back();
        receive.pop_back();
        emit sendSynkObj(obj.str); // during that phase no-one can access to the receive queue
    }
};
#endif // ONLINE_SYNCHRONIZER_H

