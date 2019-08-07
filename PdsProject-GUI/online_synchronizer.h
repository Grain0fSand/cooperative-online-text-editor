#ifndef ONLINE_SYNCHRONIZER_H
#define ONLINE_SYNCHRONIZER_H

#include <QObject>
#include <QThread>
#include <vector>
#include <QMutex>


class OnlineQuery : public QThread
{

    void run() override {
        // body of webrequest and json decode/unmarshaling
    }

    // periodic web request for filling the queue of SynkObj
};


class SynkObj {};

// for templating class is not possible split
// definition from implementation due to the
// size that the compiler must know for the type
// during the precompilation phases

class OnlineSynchronizer : public QObject
{
    Q_OBJECT

public:
    OnlineSynchronizer();

signals:
    void receiveSynkObj(SynkObj obj);

public slots:
    void sendSynkObj(SynkObj obj){
        // for thread safe implementation
        QMutexLocker l(&synk_mutex_send);

        send.push_back(obj);

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
        emit sendSynkObj(obj); // during that phase no-one can access to the receive queue
    }
};

#endif // ONLINE_SYNCHRONIZER_H

