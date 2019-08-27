#include "usertag.h"
#include <QLabel>
#include <QFileDialog>
#include <QDebug>

#define IMG_PATH "C:/Users/Dario/Desktop/PdsProject/PdsProject-GUI/PdsProject.app/Contents/Resources/.profiles/"

UserTag::UserTag(QWidget *parent) : QFrame(parent)
{
    this->setGeometry(25,0,210,71);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFrameShape(QFrame::Panel);
    this->setFrameShadow(QFrame::Raised);

    auto userLogo = new QLabel(this);
    userLogo->setGeometry(10,10,51,51);
    QPixmap logoPng(":/PdsProject.app/Contents/Resources/img/redLed.png");
    userLogo->setPixmap(logoPng);
    userLogo->setScaledContents(true);

    auto userName = new QLabel(this);
    userName->setText("Nome del collega");
    userName->setGeometry(70,14,141,21);
    QFont nameFont("Gill Sans MT",13,-1,false);
    userName->setFont(nameFont);

    auto userStatus = new QLabel(this);
    userStatus->setText("Online");
    userStatus->setGeometry(93,44,81,16);
    QFont statusFont("Tahoma",12,-1,false);
    userStatus->setFont(statusFont);

    auto userLed = new QLabel(this);
    userLed->setGeometry(77,44,16,16);
    QPixmap ledPng(":/PdsProject.app/Contents/Resources/img/greenLed");
    userLed->setPixmap(ledPng);
    userLed->setScaledContents(true);

    //auto separator = new QLine(90,50,241,66);

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




