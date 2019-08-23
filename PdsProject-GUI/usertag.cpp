#include "usertag.h"
#include <QLabel>
#include <QFileDialog>
#include <QDebug>

#define IMG_PATH "C:/Users/Dario/Desktop/PdsProject/PdsProject-GUI/PdsProject.app/Contents/Resources/img/"

UserTag::UserTag(QObject *parent) : QObject(parent)
{

}

QWidget* UserTag::createTag() {
    auto userWidget = new QWidget();
    userWidget->setGeometry(450,10,251,91);
    userWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto userLogo = new QLabel(userWidget);
    userLogo->setGeometry(10,10,71,71);
    QPixmap logoPng(":/PdsProject.app/Contents/Resources/img/redLed.png");
    userLogo->setPixmap(logoPng);
    userLogo->setScaledContents(true);

    auto userName = new QLabel(userWidget);
    userName->setText("Dario Patti");
    userName->setGeometry(90,10,151,41);
    QFont nameFont("Tahoma",20,-1,false);
    userName->setFont(nameFont);

    auto userStatus = new QLabel(userWidget);
    userStatus->setText("Online");
    userStatus->setGeometry(130,55,111,31);
    QFont statusFont("Tahoma",12,-1,false);
    userStatus->setFont(statusFont);

    auto userLed = new QLabel(userWidget);
    userLed->setGeometry(90,60,20,20);
    QPixmap ledPng(":/PdsProject.app/Contents/Resources/img/greenLed");
    userLed->setPixmap(ledPng);
    userLed->setScaledContents(true);

    //auto separator = new QLine(90,50,241,66);

    return userWidget;
    //da continuare
}

void UserTag::setUserName(QString newName)
{
    this->userName = newName;
}

void UserTag::chooseUserLogo()
{
    QString file_path = QFileDialog::getOpenFileName(nullptr,"Choose your own avatar..");
    if(file_path!="") {
        QString file_name = file_path.split("/").last();
        QString new_file_path = IMG_PATH+file_name;
        QFile::copy(file_path,new_file_path);
        this->userLogoPath = new_file_path;
    }
}

void UserTag::setUserStatus(bool newStatus)
{
    this->userStatus = newStatus;
}

QString UserTag::getUserName()
{
    return this->userName;
}

QString UserTag::getUserLogoPath()
{
    return this->userLogoPath;
}

bool UserTag::getUserStatus()
{
    return this->userStatus;
}




