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
#include <QListView>
#include <QStringListModel>
#include <QLayout>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    ui->loginErrorLabel->setVisible(false);
    this->setWindowFlag(Qt::FramelessWindowHint);

    auto settingsMenu = new QMenu;
    auto loader = new QMovie();
    settingsMenu->addAction(ui->actionChangeUsername);
    settingsMenu->addAction(ui->actionChangePassword);
    settingsMenu->addAction(ui->actionChangeAvatar);
    ui->loggedSettingsButton->setMenu(settingsMenu);
    ui->informationWhiteFrame->setVisible(false);
    ui->informationImage->setMovie(loader);
    ui->loginButton->setAutoDefault(false);

    connect(ui->loginButton,&QPushButton::clicked,this,&LoginWindow::tryLogin);
    connect(ui->closeButton,&QPushButton::pressed,this,&LoginWindow::slowClose);
    connect(ui->minimizeButton,&QPushButton::pressed,this,&LoginWindow::showMinimized);
    connect(ui->loginText,&QPushButton::clicked,this,&LoginWindow::switchFrame);
    connect(ui->registerText,&QPushButton::clicked,this,&LoginWindow::switchFrame);
    connect(ui->registerButton,&QPushButton::clicked,this,&LoginWindow::tryRegister);
    connect(ui->loggedLogoutButton,&QPushButton::clicked,[&](){this->loginCorrect=false;});
    connect(ui->loggedLogoutButton,&QPushButton::clicked,this,&LoginWindow::switchFrame);
    connect(ui->actionChangeUsername,&QAction::triggered,this,&LoginWindow::changeYourUsername);
    connect(ui->actionChangePassword,&QAction::triggered,this,&LoginWindow::changeYourPassword);
    connect(ui->actionChangeAvatar,&QAction::triggered,this,&LoginWindow::changeYourAvatar);
    connect(ui->loggedNewButton,&QPushButton::clicked,this,&LoginWindow::createDocument);
    connect(ui->loggedOpenButton,&QPushButton::clicked,this,&LoginWindow::openDocument);
    connect(ui->loggedURIButton,&QPushButton::clicked,this,&LoginWindow::requestURI);

    connect(ui->loginUsernameInput,&QLineEdit::returnPressed,ui->loginButton,&QPushButton::click);
    connect(ui->loginPasswordInput,&QLineEdit::returnPressed,ui->loginButton,&QPushButton::click);
    connect(ui->registerEmailInput,&QLineEdit::returnPressed,ui->registerButton,&QPushButton::click);
    connect(ui->registerUsernameInput,&QLineEdit::returnPressed,ui->registerButton,&QPushButton::click);
    connect(ui->registerPasswordInput,&QLineEdit::returnPressed,ui->registerButton,&QPushButton::click);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

bool LoginWindow::validateEmail(QString email)
{
    email = email.toLower();

    QRegExp regex(R"(\b[a-z0-9._%+-]+@[a-z0-9.-]+\.[a-z]{2,4}\b)");

    return regex.exactMatch(email);
}

bool LoginWindow::isLoginCorrect() {
    return this->loginCorrect;
}

void LoginWindow::showLoading(Frame frame) {

    int xPos=0;
    QString text;
    if(frame == Frame::Login) {
        xPos=320;
        text = "Logging in...";
        ui->loginUsernameInput->clearFocus();
        ui->loginPasswordInput->clearFocus();
    }
    else if(frame == Frame::Registration) {
        xPos=640;
        text = "Signing up...";
        ui->registerEmailInput->clearFocus();
        ui->registerUsernameInput->clearFocus();
        ui->registerPasswordInput->clearFocus();
    }

    ui->informationWhiteFrame->setGeometry(xPos,0,320,480);

    ui->informationButton->setVisible(false);
    ui->informationLabel->setStyleSheet("color:darkblue");
    ui->informationLabel->setText(text);
    ui->informationImage->movie()->setFileName(":/resources/loader.gif");
    ui->informationImage->setGeometry(QRect(110, 270, 100, 100));
    ui->informationImage->movie()->start();
    ui->informationImage->setVisible(true);

    ui->informationWhiteFrame->setVisible(true);
}

void LoginWindow::resetInputFields()
{
    ui->loginUsernameInput->clear();
    ui->loginPasswordInput->clear();
    ui->loginErrorLabel->clear();
    ui->registerEmailInput->clear();
    ui->registerPasswordInput->clear();
    ui->registerErrorLabel->clear();
    if(sender()->objectName()!="switchAnimation")
        ui->registerUsernameInput->clear();
}

void LoginWindow::slowClose()
{
    auto a = new QPropertyAnimation(this, "windowOpacity");
    a->setDuration(500);
    a->setStartValue(this->windowOpacity());
    a->setEndValue(0.0);
    a->start(QAbstractAnimation::DeleteWhenStopped);

    connect(a,&QPropertyAnimation::finished,this,&LoginWindow::close);
}

LoginWindow::Frame LoginWindow::currentVisibleFrame()
{
    int x = ui->informationWhiteFrame->x();

    switch(x) {
        case 0:
            return Frame::PersonalPage;
        case 320:
            return Frame::Login;
        case 640:
            return Frame::Registration;
        default:
            break;
    }
    return Frame::Login;
};

void LoginWindow::switchFrame(int direction)
{
    if(direction==0) {
        if(this->sender()->objectName()=="loginText" || this->sender()->objectName()=="loggedLogoutButton") {
            direction=-1;
        }
        else if(this->sender()->objectName()=="registerText" || this->sender()->objectName()=="informationButton") {
            direction=1;
        }
    }

    ui->loginErrorLabel->clear();

    QRect frameGeometry = ui->formFrame->geometry();
    frameGeometry.setX(frameGeometry.x()+320*direction);
    auto switchAnimation = new QPropertyAnimation(ui->formFrame, "geometry");
    switchAnimation->setObjectName("switchAnimation");
    switchAnimation->setDuration(400);
    switchAnimation->setStartValue(ui->formFrame->geometry());
    switchAnimation->setEndValue(frameGeometry);

    connect(switchAnimation,&QPropertyAnimation::finished,this,&LoginWindow::resetInputFields);

    if(this->sender()->objectName()=="informationButton") {
        connect(switchAnimation,&QPropertyAnimation::finished,this,[&](){
            ui->informationWhiteFrame->setVisible(false);
            disconnect(ui->informationButton,&QPushButton::clicked,this,nullptr);
            ui->loginUsernameInput->setText(ui->registerUsernameInput->text());
            ui->registerUsernameInput->clear();
            ui->informationButton->setGeometry(110, 350, 101, 31);
        });
    }
    switchAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoginWindow::changeYourUsername()
{
    QInputDialog dialog(this);
    dialog.setLabelText("Change your username:");
    dialog.setTextValue(ui->loggedUsernameLabel->text());
    dialog.setModal(true);
    while(true) {
        if(dialog.exec()==QDialog::Accepted) {
            if(dialog.textValue()=="") {
                QMessageBox advice(this);
                advice.setText("You left the field empty!\nEnter a valid username");
                advice.setIcon(QMessageBox::Critical);
                advice.exec();
            }
            else {
                this->sessionData.username = dialog.textValue().toStdString();
                QString encodedAvatar = LoginWindow::generateEncodedImage(this->sessionData.avatar);
                QString username = dialog.textValue();
                QString password = QString::fromStdString(this->sessionData.password);

                QThread* sender = new OnlineSender(this->sessionData.token, username, encodedAvatar, password);
                sender->start();

                break;
            }
        }
    }
}

void LoginWindow::changeYourAvatar()
{
    QString avatar_path = QFileDialog::getOpenFileName(nullptr,"Choose your own avatar..");
    if(avatar_path!="") {

        QPixmap avatar(avatar_path);
        this->sessionData.avatar = avatar;
        QString encodedAvatar = LoginWindow::generateEncodedImage(avatar);
        QString username = ui->loggedUsernameLabel->text();
        QString password = QString::fromStdString(this->sessionData.password);

        QThread* sender = new OnlineSender(this->sessionData.token, username, encodedAvatar, password);
        sender->start();
    }
}

void LoginWindow::changeYourPassword()
{
    QInputDialog dialog(this);
    dialog.setLabelText("Insert a new password:");
    dialog.setTextEchoMode(QLineEdit::Password);
    dialog.setModal(true);
    while(true) {
        if(dialog.exec()==QDialog::Accepted) {
            if(dialog.textValue()=="") {
                QMessageBox advice(this);
                advice.setText("You left the field empty!\nEnter a valid password");
                advice.setIcon(QMessageBox::Critical);
                advice.exec();
            }
            else {
                QString username = QString::fromStdString(this->sessionData.username);
                QString encodedAvatar = LoginWindow::generateEncodedImage(this->sessionData.avatar);
                QString password = dialog.textValue();

                QThread* sender = new OnlineSender(this->sessionData.token, username, encodedAvatar, password);
                sender->start();

                break;
            }
        }
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
                this->sessionData.docName = dialog.textValue().toStdString();

                QThread* sender = new OnlineSender(this->sessionData.token, this->sessionData.docName);
                sender->start();
                break;
            }
        }
        else return;
    }
}

void LoginWindow::openDocument()
{
    auto docListDialog = new QDialog(this);
    auto vertLayout= new QVBoxLayout(docListDialog);
    docListDialog->setLayout(vertLayout);
    auto docListView = new QListView(docListDialog);
    docListDialog->layout()->addWidget(docListView);
    auto widget = new QWidget(docListDialog);
    docListDialog->layout()->addWidget(widget);
    auto horizLayout = new QHBoxLayout(widget);
    widget->setLayout(horizLayout);
    auto okButton = new QPushButton(widget);
    auto cancButton = new QPushButton(widget);
    widget->layout()->addWidget(okButton);
    widget->layout()->addWidget(cancButton);

    QStringList list;
    list << "Nome1" << "Nome2" << "molto bene!";
    auto model = new QStringListModel(this);
    model->setStringList(list);
    docListView->setModel(model);

    //docListDialog->setWindowFlags(Qt::FramelessWindowHint);
    docListDialog->setGeometry(this->x()+320,this->y()+240,320,240);
    docListDialog->autoFillBackground();
    okButton->setText("Ok");
    cancButton->setText("Cancel");

    QModelIndex fakeIndex;
    docListView->setCurrentIndex(fakeIndex);

    connect(okButton,&QPushButton::clicked,this,[&](){
        if(!docListView->currentIndex().isValid()) {
            QMessageBox advice(this);
            advice.setText("You have not selected any document!");
            advice.setIcon(QMessageBox::Critical);
            advice.exec();
        }
        else {
            //richiedi documento al server

            //richiedi lista partecipanti
            QThread* sender = new OnlineSender(this->sessionData.token, QString::number(docListView->currentIndex().row()+1));
            sender->start();

            docListDialog->close();
        }
    });
    connect(cancButton,&QPushButton::clicked,docListDialog,&QDialog::close);

    docListDialog->exec();

    //delete docListDialog;
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

    if(username.isEmpty()) {
        ui->loginErrorLabel->setText("Username field is empty");
    }
    else if(password.isEmpty()) {
        ui->loginErrorLabel->setText("Password field is empty");
    }
    else if(username=="test2" && password=="test2") {
        this->loginCorrect=true;
        ui->loggedUsernameLabel->setText(username);
        ui->loggedAvatar->setPixmap(QPixmap(":/resources/avatar.png"));
        this->switchFrame(1);
    }
    else {
        QThread* sender = new OnlineSender(username, password);
        sender->start();

        showLoading(Frame::Login);
    }
    ui->loginErrorLabel->setVisible(true);
}

//TODO: (optional) first check email and username on DB, if ok let choose an image
void LoginWindow::tryRegister()
{
    QString email(ui->registerEmailInput->text());
    QString username(ui->registerUsernameInput->text());
    QString password(ui->registerPasswordInput->text());
    QString avatar_path(":/resources/avatar.png");

    if(email.isEmpty())
        ui->registerErrorLabel->setText("Email field is empty");
    else if(!validateEmail(email))
        ui->registerErrorLabel->setText("Insert a valid email address");
    else if(username.isEmpty())
        ui->registerErrorLabel->setText("Username field is empty");
    else if(password.isEmpty())
        ui->registerErrorLabel->setText("Password field is empty");
    else
    {
        ui->registerErrorLabel->clear();

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
        QPixmap avatar(avatar_path);
        QString encodedAvatar = LoginWindow::generateEncodedImage(avatar);

        QThread* sender = new OnlineSender(email, username, password, encodedAvatar);
        sender->start();

        showLoading(Frame::Registration);
    }
    ui->registerErrorLabel->setVisible(true);
}

void LoginWindow::showRegisterResponse(bool goodResponse, QString responseText)
{
    ui->informationImage->movie()->stop();
    ui->informationLabel->setText(responseText);
    ui->informationButton->setDefault(true);
    ui->informationImage->setGeometry(QRect(140, 284, 40, 40));

    if(goodResponse) {
        connect(ui->informationButton,&QPushButton::clicked,this,&LoginWindow::switchFrame);
        ui->informationLabel->setStyleSheet("color:darkgreen;");
        ui->informationButton->setText("Go to Login");
        ui->informationImage->movie()->setFileName(":/resources/correct_icon.gif");
    }
    else {
        connect(ui->informationButton,&QPushButton::clicked,this,[&](){
            ui->informationWhiteFrame->setVisible(false);
            ui->registerPasswordInput->setFocus();
            disconnect(ui->informationButton,&QPushButton::clicked,this,nullptr);
        });
        ui->informationLabel->setStyleSheet("color:darkred;");
        ui->informationButton->setText("Try Again");
        ui->informationImage->movie()->setFileName(":/resources/error_icon.gif");
    }
    ui->informationButton->setVisible(true);
    ui->informationImage->movie()->start();
    ui->informationLabel->setVisible(true);
    ui->informationWhiteFrame->setVisible(true);
}

void LoginWindow::showLoginResponse(bool goodResponse, QString responseText,  QString replyString)
{
    ui->informationImage->movie()->stop();
    ui->informationLabel->setText(responseText);
    ui->informationButton->setDefault(true);
    ui->informationImage->setGeometry(QRect(140, 284, 40, 40));

    if(goodResponse) {
        connect(ui->informationButton,&QPushButton::clicked,this,&LoginWindow::switchFrame);
        ui->informationLabel->setStyleSheet("color:darkgreen;");
        ui->informationButton->setText("Go to your page");
        ui->informationButton->setGeometry(100,350,121,31);
        ui->informationImage->movie()->setFileName(":/resources/correct_icon.gif");

        QStringList replyParts = replyString.split(":");
        this->sessionData.avatar = recoverImageFromEncodedString(replyParts[0]);
        this->sessionData.token = replyParts[1].toStdString();
        this->docsList = replyParts[2].split("|",QString::SkipEmptyParts);

        this->sessionData.username = ui->loginUsernameInput->text().toStdString();
        ui->loggedUsernameLabel->setText(ui->loginUsernameInput->text());
        ui->loggedAvatar->setPixmap(this->sessionData.avatar);
    }
    else {
        connect(ui->informationButton,&QPushButton::clicked,this,[&](){
            ui->informationWhiteFrame->setVisible(false);
            ui->loginPasswordInput->setFocus();
            disconnect(ui->informationButton,&QPushButton::clicked,this,nullptr);
        });
        ui->informationLabel->setStyleSheet("color:darkred;");
        ui->informationButton->setText("Try Again");
        ui->informationImage->movie()->setFileName(":/resources/error_icon.gif");
    }
    ui->informationButton->setVisible(true);
    ui->informationImage->movie()->start();
    ui->informationLabel->setVisible(true);
    ui->informationWhiteFrame->setVisible(true);
}

void LoginWindow::showNewDocResponse(bool goodResponse, QString responseText, QString replyString)
{
    QMessageBox advice(this);
    advice.setText(responseText);
    if(goodResponse)
        advice.setIcon(QMessageBox::Information);
    else
        advice.setIcon(QMessageBox::Warning);

    advice.exec();

    if(goodResponse) {
        QStringList replyParts = replyString.split("|");
        this->sessionData.docId = replyParts[0].toStdString();
        this->sessionData.status = true;
        this->loginCorrect=true;

        close();
    }
}

void LoginWindow::showOpenDocResponse(QString responseString)
{
    //TODO: fake response, remove when it's time
    responseString = "1"
                     "|dario:iVBORwAAAACXBIWXMAAC4jAAAuIwF4pT92AAAFzUlEQoge1ZW28bRRT:1"
                     "|inanimat:iVBORw0KGgoAAAIWXMAAC4jAAAF4pT92AzUlEQVRoge1ZW28bRRT:0"
                     "|eliax1996:iVBORw0KGgoAA0jAAAuIwF4pT92AAAFzUlEQVRoge1ZW28bRRT:0";

    if(responseString[0]=='1') {
        responseString = responseString.remove(0, 2);
        QStringList users = responseString.split("|");
        for (QString user : users) {
            QStringList userElements = user.split(":");
            UserTag userTag;
            userTag.setUsername(userElements[0]);

            //TODO: also remove this, and uncomment the last row
            QPixmap fakePixmap = this->sessionData.avatar;
            userTag.setAvatar(fakePixmap);
            //userTag.setAvatar(recoverImageFromEncodedString(userElements[1]));

            userTag.setUserStatus(userElements[2].toInt() != 0);
            userTag.setUserColor(chooseColorFromString(userElements[0]));
            this->sessionData.usersList.push_back(userTag);
        }
        this->sessionData.status = true;
        this->loginCorrect=true;

        close();
    }
}

void LoginWindow::showUpdateUserDataResponse(bool goodResponse, QString responseText)
{
    QMessageBox advice(this);
    advice.setText(responseText);
    if(goodResponse)
        advice.setIcon(QMessageBox::Information);
    else
        advice.setIcon(QMessageBox::Warning);

    advice.exec();

    if(goodResponse) {
        ui->loggedUsernameLabel->setText(QString::fromStdString(this->sessionData.username));
        ui->loggedAvatar->setPixmap(this->sessionData.avatar);
    }
    else {
        this->sessionData.username = ui->loggedUsernameLabel->text().toStdString();
        this->sessionData.avatar = *ui->loggedAvatar->pixmap();
    }
}

QString LoginWindow::generateEncodedImage(QPixmap avatar) {

    avatar = avatar.scaled(71,71);
    QByteArray blobAvatar;
    QBuffer buffer(&blobAvatar);
    buffer.open(QIODevice::WriteOnly);
    avatar.save(&buffer, "PNG");
    QString encodedAvatar = buffer.data().toBase64(QByteArray::Base64UrlEncoding);

    return encodedAvatar;
}

QPixmap LoginWindow::recoverImageFromEncodedString(const QString& code) {

    QByteArray array = QByteArray::fromBase64(code.toUtf8(),QByteArray::Base64UrlEncoding);
    QPixmap avatar;
    avatar.loadFromData(array);

    return avatar;
}

QColor LoginWindow::chooseColorFromString(QString string) {

    std::hash<std::string> hash;
    auto hashed = hash(string.toStdString());
    QString colorString = QString::number(((hashed>>24)&0xFF),16)+
                          QString::number(((hashed>>16)&0xFF),16)+
                          QString::number(((hashed>>8)&0xFF),16)+
                          QString::number((hashed&0xFF),16);

    //QColor color("#"+colorString);
    QColor color = QColor::fromCmyk((hashed>>24)&0xFF,(hashed>>16)&0xFF,(hashed>>8)&0xFF,(hashed>>0)&0xFF);

    return color;
}