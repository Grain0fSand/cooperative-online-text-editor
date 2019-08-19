#ifndef IMAGE_UTILITY_H
#define IMAGE_UTILITY_H

#include <QBitmap>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QUrl>

// utility for download image from web
QPixmap download_from(const QString& url){

    QNetworkAccessManager nam;
    QEventLoop loop;
    QObject::connect(&nam,&QNetworkAccessManager::finished,&loop,&QEventLoop::quit);
    QNetworkReply *reply = nam.get(QNetworkRequest(url));
    loop.exec();

    QPixmap pm;
    pm.loadFromData(reply->readAll());

    delete reply;
    return pm;
}

QUrl create_svg(QColor color,int user_id){
    QString rgb = color.toRgb().name();
    qDebug() << rgb << " user id: " << user_id;


    QUrl url;



    return url;
}

#endif // IMAGE_UTILITY_H
