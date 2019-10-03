#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mytextedit.h"
#include <QDebug>
#include <QBuffer>
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
    ui->registerWhiteFrame->setVisible(false);

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

void LoginWindow::resetInputFields() {
    QList<QLineEdit *> inputList= ui->formFrame->findChildren<QLineEdit *>(QString(), Qt::FindDirectChildrenOnly);
    for(QLineEdit* input : inputList) {
        input->setText("");
    }
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
        else if(this->sender()->objectName()=="registerText" || this->sender()->objectName()=="registerInformationButton") {
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

    if(this->sender()->objectName()=="registerInformationButton") {
        connect(switchAnimation,&QPropertyAnimation::finished,this,[&](){
            ui->registerWhiteFrame->setVisible(false);
            disconnect(ui->registerInformationButton,&QPushButton::clicked,this,nullptr);
        });
    }
    connect(switchAnimation,&QPropertyAnimation::finished,this,&LoginWindow::resetInputFields);
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
                MyTextEdit::getInstance().setDocumentName(dialog.textValue());
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
    QString username = ui->loginUsernameInput->text();
    QString password = ui->loginPasswordInput->text();

    if(username=="") {
        ui->loginErrorLabel->setText("Insert a valid username");
    }
    else if(password=="") {
        ui->loginErrorLabel->setText("Insert a password");
    }
    else if(username!="test" || password!="test") {
        ui->loginErrorLabel->setText("Login failed\nWrong username or password");
    }
    else {
        QThread* sender = new OnlineSender(username, password);
        sender->start();

        this->loginCorrect=true;
        ui->loggedUsernameLabel->setText(username);
        ui->loggedAvatar->setPixmap(QPixmap(":/resources/avatar.png"));
        this->switchFrame(1);
    }
    ui->loginErrorLabel->setVisible(true);
}

//TODO: check on empty fields
//TODO: check on the email validity
//TODO: (optional) first check email and username on DB, if ok let choose an image
void LoginWindow::tryRegister()
{
    QString email(ui->registerEmailInput->text());
    QString username(ui->registerUsernameInput->text());
    QString password(ui->registerPasswordInput->text());
    QString avatar_path(":/resources/avatar.png");

    QMessageBox::StandardButton chooseAvatar;
    chooseAvatar = QMessageBox::question(this, "Choose your avatar",
            "Do you want to choose a personal avatar?\nClicking 'No', a default one will be assigned to you.",
            QMessageBox::Yes|QMessageBox::No);
    if (chooseAvatar == QMessageBox::Yes) {
        avatar_path = QFileDialog::getOpenFileName(nullptr,"Choose your own avatar...");
        if(avatar_path=="") {
            avatar_path = ":/resources/avatar.png";
            QMessageBox advice(this);
            advice.setText("You didn't select any image!");
            advice.setInformativeText("<― this avatar will be assigned to you.");
            advice.setIconPixmap(QPixmap(":/resources/avatar.png").scaled(60,60));
            advice.exec();
        }
    }

    QString encodedAvatar = LoginWindow::generateBlob(avatar_path);

    QThread* sender = new OnlineSender(email, username, password, encodedAvatar);
    sender->start();
}

void LoginWindow::getRegisterResponse(bool good_response, QString response_text)
{
    ui->registerInformationLabel->setText(response_text);
    if(good_response) {
        connect(ui->registerInformationButton,&QPushButton::clicked,this,&LoginWindow::switchFrame);
        ui->registerInformationLabel->setStyleSheet("color:green;");
        ui->registerInformationButton->setText("Go to Login");
    }
    else {
        connect(ui->registerInformationButton,&QPushButton::clicked,this,[&](){
            ui->registerWhiteFrame->setVisible(false);
            disconnect(ui->registerInformationButton,&QPushButton::clicked,this,nullptr);
        });
        ui->registerInformationLabel->setStyleSheet("color:red;");
        ui->registerInformationButton->setText("Try Again");
    }
    ui->registerWhiteFrame->setVisible(true);
}

QString LoginWindow::generateBlob(const QString& avatar_path) {

    QString format(avatar_path.split('.').last().toUpper());
    QPixmap avatar(avatar_path);
    avatar = avatar.scaled(71,71);
    QByteArray blobAvatar;
    QBuffer buffer(&blobAvatar);
    buffer.open(QIODevice::WriteOnly);
    avatar.save(&buffer, "PNG"); //format.toStdString().c_str()
    QString encodedAvatar = buffer.data().toBase64(QByteArray::Base64UrlEncoding);

    return encodedAvatar;
}
