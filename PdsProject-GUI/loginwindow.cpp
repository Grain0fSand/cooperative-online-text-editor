#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QDebug>
#include <QPropertyAnimation>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    ui->loginErrorLabel->setVisible(false);
    this->setWindowFlag(Qt::FramelessWindowHint);


    connect(ui->loginButton,&QPushButton::clicked,this,&LoginWindow::tryLogin);
    connect(ui->closeButton,&QPushButton::pressed,this,&LoginWindow::slowClose);
    connect(ui->minimizeButton,&QPushButton::pressed,this,&LoginWindow::showMinimized);
    connect(ui->loginText,&QPushButton::clicked,this,&LoginWindow::switchFrame);
    connect(ui->registerText,&QPushButton::clicked,this,&LoginWindow::switchFrame);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

bool LoginWindow::isLoginCorrect()
{
    return this->loginCorrect;
}

void LoginWindow::slowClose()
{
    QPropertyAnimation* a = new QPropertyAnimation(this, "windowOpacity");
    a->setDuration(500);
    a->setStartValue(this->windowOpacity());
    a->setEndValue(0.0);
    a->start();

    connect(a,&QPropertyAnimation::finished,this,&LoginWindow::close);
}

void LoginWindow::switchFrame()
{
    int direction=0;
    if(this->sender()->objectName()=="loginText") direction=-1;
    else if(this->sender()->objectName()=="registerText") direction=1;

    QRect frameGeometry = ui->formFrame->geometry();
    frameGeometry.setX(frameGeometry.x()+320*direction);
    QPropertyAnimation* switchAnimation = new QPropertyAnimation(ui->formFrame, "geometry");
    switchAnimation->setDuration(400);
    switchAnimation->setStartValue(ui->formFrame->geometry());
    switchAnimation->setEndValue(frameGeometry);

    switchAnimation->start();
}

void LoginWindow::tryLogin()
{
    if(ui->loginUsernameInput->text()=="") {
        ui->loginErrorLabel->setText("Insert a valid username");
    }
    else if(ui->loginPasswordInput->text()=="") {
        ui->loginErrorLabel->setText("Insert a password");
    }
    else if(ui->loginUsernameInput->text()!="test" || ui->loginPasswordInput->text()!="test") {
        ui->loginErrorLabel->setText("Login Failed");
    }
    else {
        this->loginCorrect=true;
        close();
    }
    ui->loginErrorLabel->setVisible(true);
}

void LoginWindow::mousePressEvent(QMouseEvent *event) {
    mouseClickXCoordinate = event->x();
    mouseClickYCoordinate = event->y();
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event) {
    if(event->localPos().x()<=ui->titleBar->width() && event->localPos().y()<=ui->titleBar->height()) {
        move(event->globalX()-mouseClickXCoordinate,event->globalY()-mouseClickYCoordinate);
        qDebug() << event->localPos();
    }
}
