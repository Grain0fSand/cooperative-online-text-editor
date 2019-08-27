#ifndef USERTAG_H
#define USERTAG_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include "mainwindow.h"

class UserTag : public QFrame
{
    Q_OBJECT

public:
    explicit UserTag(QWidget *parent = nullptr);
    ~UserTag() = default;

    void setUserName(QString);
    void chooseUserLogo();
    void setUserStatus(bool);
    QString getUserName();
    QString getUserLogoPath();
    bool getUserStatus();

signals:

public slots:

private:
    QString userName;
    QString userLogoPath;
    bool userStatus;
};

#endif // USERTAG_H
