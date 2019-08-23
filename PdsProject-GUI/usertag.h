#ifndef USERTAG_H
#define USERTAG_H

#include <QObject>
#include <QWidget>

class UserTag : public QObject
{
    Q_OBJECT
public:
    explicit UserTag(QObject *parent = nullptr);
    ~UserTag() = default;

    QWidget* createTag();
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
