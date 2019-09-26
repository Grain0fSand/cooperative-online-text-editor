#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mytextedit.h"
#include <cmath>
#include <QDebug>
#include <QPropertyAnimation>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    ui->loginErrorLabel->setVisible(false);
    this->setWindowFlag(Qt::FramelessWindowHint);

    QMenu* settingsMenu = new QMenu;
    settingsMenu->addAction(ui->actionChangeUsername);
    settingsMenu->addAction(ui->actionChangeAvatar);
    ui->loggedSettingsButton->setMenu(settingsMenu);

    connect(ui->loginButton,&QPushButton::clicked,this,&LoginWindow::tryLogin);
    connect(ui->closeButton,&QPushButton::pressed,this,&LoginWindow::slowClose);
    connect(ui->minimizeButton,&QPushButton::pressed,this,&LoginWindow::showMinimized);
    connect(ui->loginText,&QPushButton::clicked,this,&LoginWindow::switchFrame);
    connect(ui->registerText,&QPushButton::clicked,this,&LoginWindow::switchFrame);
    connect(ui->registerButton,&QPushButton::clicked,this,&LoginWindow::tryRegister);
    connect(ui->loggedLogoutButton,&QPushButton::clicked,[&](){this->loginCorrect=false;});
    connect(ui->loggedLogoutButton,&QPushButton::clicked,this,&LoginWindow::switchFrame);
    connect(ui->actionChangeUsername,&QAction::triggered,this,&LoginWindow::changeYourUsername);
    connect(ui->actionChangeAvatar,&QAction::triggered,this,&LoginWindow::changeYourAvatar);
    connect(ui->loggedNewButton,&QPushButton::clicked,this,&LoginWindow::createDocument);
    connect(ui->loggedOpenButton,&QPushButton::clicked,this,&LoginWindow::openDocument);
    connect(ui->loggedURIButton,&QPushButton::clicked,this,&LoginWindow::requestURI);
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
    a->start(QAbstractAnimation::DeleteWhenStopped);

    connect(a,&QPropertyAnimation::finished,this,&LoginWindow::close);
}

void LoginWindow::switchFrame(int direction)
{
    if(direction==0) {
        if(this->sender()->objectName()=="loginText" || this->sender()->objectName()=="loggedLogoutButton") {
            direction=-1;
        }
        else if(this->sender()->objectName()=="registerText") {
            direction=1;
        }
    }

    ui->loginErrorLabel->clear();

    QRect frameGeometry = ui->formFrame->geometry();
    frameGeometry.setX(frameGeometry.x()+320*direction);
    QPropertyAnimation* switchAnimation = new QPropertyAnimation(ui->formFrame, "geometry");
    switchAnimation->setDuration(400);
    switchAnimation->setStartValue(ui->formFrame->geometry());
    switchAnimation->setEndValue(frameGeometry);

    switchAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoginWindow::changeYourUsername()
{
    QInputDialog dialog(this);
    dialog.setLabelText("Change your username:");
    dialog.setTextValue(ui->loggedUsernameLabel->text());
    dialog.setModal(true);
    while(true) {
        if(dialog.exec()==QDialog::Accepted && dialog.textValue()=="") {
            QMessageBox advice(this);
            advice.setText("Username field empty!\nEnter a valid username");
            advice.setIcon(QMessageBox::Critical);
            advice.exec();
        }
        else {
            ui->loggedUsernameLabel->setText(dialog.textValue());
            break;
        }
    }
}

void LoginWindow::changeYourAvatar()
{
    QString file_path = QFileDialog::getOpenFileName(nullptr,"Choose your own avatar..");
    if(file_path!="") {
        QString file_name = file_path.split("/").last();
        QString new_file_path = ":/resources/"+file_name;
        QFile::copy(file_path,new_file_path);

        ui->loggedAvatar->setPixmap(QPixmap(new_file_path));
    }
}

void LoginWindow::createDocument()
{
    QInputDialog dialog(this);
    dialog.setLabelText("Insert filename:");

    while(true) {
        if(dialog.exec()==QDialog::Accepted) {
            if(dialog.textValue()=="") {
                QMessageBox advice(this);
                advice.setText("Filename field empty!\nEnter a valid one");
                advice.setIcon(QMessageBox::Critical);
                advice.exec();
            } else {
                myTextEdit::getInstance().setDocumentName(dialog.textValue());
                QMessageBox advice(this);
                advice.setText("New document created on the server!");
                advice.setIcon(QMessageBox::Information);
                advice.exec();
                break;
            }
        }
        else return;
    }
    close();
}

void LoginWindow::openDocument()
{
    //i have to show a simple list received from db:
    //filename | creation data | last mod data | num of users
}

void LoginWindow::requestURI()
{
    QInputDialog dialog(this);
    dialog.setLabelText("Paste the URI here:");
    dialog.setModal(true);
    while(true) {
        if(dialog.exec()==QDialog::Accepted && dialog.textValue()=="") {
            QMessageBox advice(this);
            advice.setText("URI field empty!\nEnter a valid URI");
            advice.setIcon(QMessageBox::Critical);
            advice.exec();
        }
        else {
            qDebug() << "open document from URI";
            break;
        }
    }
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
        ui->loginErrorLabel->setText("Login failed\nWrong username or password");
    }
    else {
        this->loginCorrect=true;
        ui->loggedUsernameLabel->setText(ui->loginUsernameInput->text());
        this->switchFrame(1);
    }
    ui->loginErrorLabel->setVisible(true);
}

void LoginWindow::tryRegister()
{
    //TODO: check in db if the username exist already
    //if not, proceed with the registration, otherwise show a message

    QPixmap avatar(":/resources/avatar.png");
    QMessageBox::StandardButton chooseAvatar;
    chooseAvatar = QMessageBox::question(this, "Choose your avatar",
            "Do you want to choose a personal avatar?\nClicking 'No' a default one will be assigned to you",
            QMessageBox::Yes|QMessageBox::No);
    if (chooseAvatar == QMessageBox::Yes) {
        QString file_path = QFileDialog::getOpenFileName(nullptr,"Choose your own avatar...");
        if(file_path!="") {
            QString file_name = file_path.split("/").last();
            QString new_file_path = ":/resources/"+file_name;
            QFile::copy(file_path,new_file_path);
            avatar.load(new_file_path);
        } else {
            QMessageBox advice(this);
            advice.setText("You didn't select any image!");
            advice.setInformativeText("<― this avatar will be assigned to you");
            advice.setIconPixmap(QPixmap(":/resources/avatar.png").scaled(60,60));
            advice.exec();
        }
    }
    //now the avatar is ready to be set
    //TODO: if everything ok, proceed with registration on db


    /*
    //formula for the color (experimental... but trust me)

    int x,n,group,hue,sat,val;

    x = 1;
    n = ceil(log2(((x-1)%16)+1));
    hue = ceil(((2*((x-1)%16)+1)/pow(2,n)-1)*360);
    group = floor((x-1)/16);

    switch(group)
    {
        case 0: //first 16 users
            sat = 35;
            val = 75;
            break;
        case 1: //17-32
            sat = 100;
            val = 100;
            break;
        case 2: //33-48
            sat = 35;
            val = 100;
            break;
        case 3: //49-64
            sat = 100;
            val = 75;
            break;
        default: //other users
            std::srand(std::time(nullptr));
            hue = rand()%360;
            sat = rand()%256;
            val = rand()%256;
            break;
    }
    QString hexColor = QColor::fromHsv(hue,sat,val).name();
     */
}

void LoginWindow::mousePressEvent(QMouseEvent *event) {
    mouseClickXCoordinate = event->x();
    mouseClickYCoordinate = event->y();
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event) {
    if(event->localPos().x()<=ui->titleBar->width() && event->localPos().y()<=ui->titleBar->height()) {
        move(event->globalX()-mouseClickXCoordinate,event->globalY()-mouseClickYCoordinate);
    }
}
