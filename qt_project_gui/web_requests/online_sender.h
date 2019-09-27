//
// Created by Elia Migliore on 27/09/2019.
//

#ifndef TRANSLATOR_ONLINE_SENDER_H
#define TRANSLATOR_ONLINE_SENDER_H

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

#endif //TRANSLATOR_ONLINE_SENDER_H
