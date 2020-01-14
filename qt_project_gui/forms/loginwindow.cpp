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
    SessionData::accessToSessionData().loginWindowPointer = this;
    SessionData::accessToSessionData().isLoginCorrect = false;

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
    connect(ui->actionChangeUsername,&QAction::triggered,this,&LoginWindow::changeYourUsername);
    connect(ui->actionChangePassword,&QAction::triggered,this,&LoginWindow::changeYourPassword);
    connect(ui->actionChangeAvatar,&QAction::triggered,this,&LoginWindow::changeYourAvatar);
    connect(ui->loggedNewButton,&QPushButton::clicked,this,&LoginWindow::createDocument);
    connect(ui->loggedOpenButton,&QPushButton::clicked,this,&LoginWindow::openDocument);
    connect(ui->loggedURIButton,&QPushButton::clicked,this,&LoginWindow::requestURI);
    connect(ui->loggedLogoutButton,&QPushButton::clicked,[&](){SessionData::accessToSessionData().isLoginCorrect = false;});
    connect(ui->loggedLogoutButton,&QPushButton::clicked,this,&LoginWindow::switchFrame);

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
    ui->informationLabel->setStyleSheet("color:#3C505B");
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
    if(this->sender()== nullptr) {
        frameGeometry.setX(320);
        switchAnimation->setDuration(0);
    }
    else if(this->sender()->objectName()=="informationButton") {
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
                SessionData::accessToSessionData().username = dialog.textValue().toStdString();
                QString encodedAvatar = LoginWindow::generateEncodedImage(SessionData::accessToSessionData().avatar);
                QString username = dialog.textValue();
                QString password = QString::fromStdString(SessionData::accessToSessionData().password);

                QThread* sender = new OnlineSender(SessionData::accessToSessionData().token, username, encodedAvatar, password);
                sender->start();

                break;
            }
        }
        else break;
    }
}

void LoginWindow::changeYourAvatar()
{
    QString avatar_path = QFileDialog::getOpenFileName(nullptr,"Choose your own avatar..");
    if(avatar_path!="") {

        QPixmap avatar(avatar_path);
        SessionData::accessToSessionData().avatar = avatar;
        QString encodedAvatar = LoginWindow::generateEncodedImage(avatar);
        QString username = ui->loggedUsernameLabel->text();
        QString password = QString::fromStdString(SessionData::accessToSessionData().password);

        QThread* sender = new OnlineSender(SessionData::accessToSessionData().token, username, encodedAvatar, password);
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
                QString username = QString::fromStdString(SessionData::accessToSessionData().username);
                QString encodedAvatar = LoginWindow::generateEncodedImage(SessionData::accessToSessionData().avatar);
                QString password = dialog.textValue();

                QThread* sender = new OnlineSender(SessionData::accessToSessionData().token, username, encodedAvatar, password);
                sender->start();

                break;
            }
        }
        else break;
    }
}

void LoginWindow::createDocument()
{
    QInputDialog dialog(this);
    QMessageBox advice(this);
    dialog.setLabelText("Insert new document name:");

    while(true) {
        if(dialog.exec()==QDialog::Accepted) {
            if(dialog.textValue()=="") {
                advice.setText("Document name field empty!\nEnter a valid one");
            } else {
                if(this->docsList.contains(dialog.textValue())) {
                    advice.setText("A document with this name already exists!\nPlease enter another one.");
                }
                else {
                    SessionData::accessToSessionData().docName = dialog.textValue().toStdString();

                    QThread* sender = new OnlineSender(SessionData::accessToSessionData().token, SessionData::accessToSessionData().docName);
                    sender->start();
                    break;
                }
            }
            advice.setIcon(QMessageBox::Critical);
            advice.exec();
        }
        else break;
    }
}

void LoginWindow::openDocument()
{
    auto docListDialog = new QDialog(this);
    auto vertLayout= new QVBoxLayout(docListDialog);
    docListDialog->setLayout(vertLayout);
    auto docListView = new QListView(docListDialog);
    docListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    docListView->setStyleSheet("background-color:white");
    docListDialog->layout()->addWidget(docListView);
    auto widget = new QWidget(docListDialog);
    docListDialog->layout()->addWidget(widget);
    auto horizLayout = new QHBoxLayout(widget);
    widget->setLayout(horizLayout);
    auto okButton = new QPushButton(widget);
    auto cancButton = new QPushButton(widget);
    widget->layout()->addWidget(okButton);
    widget->layout()->addWidget(cancButton);

    auto model = new QStringListModel(this);
    model->setStringList(this->docsList);
    docListView->setModel(model);

    docListDialog->setGeometry(this->x()+120,this->y()+110,400,300);
    docListDialog->autoFillBackground();
    okButton->setText("Ok");
    cancButton->setText("Cancel");

    QModelIndex fakeIndex;
    docListView->setCurrentIndex(fakeIndex);

    connect(docListView,&QAbstractItemView::doubleClicked,this,[&](){
        QString docName = docListView->currentIndex().data(Qt::DisplayRole).toString();
        SessionData::accessToSessionData().docName = docName.toStdString();
        QThread* sender = new OnlineSender(SessionData::accessToSessionData().token, docName);
        sender->start();

        docListDialog->close();
    });

    connect(okButton,&QPushButton::clicked,this,[&](){
        if(!docListView->currentIndex().isValid()) {
            QMessageBox advice(this);
            advice.setText("You have not selected any document!");
            advice.setIcon(QMessageBox::Critical);
            advice.exec();
        }
        else {
            QString docName = docListView->currentIndex().data(Qt::DisplayRole).toString();
            SessionData::accessToSessionData().docName = docName.toStdString();
            QThread* sender = new OnlineSender(SessionData::accessToSessionData().token, docName);
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
    QMessageBox advice(this);

    dialog.setLabelText("Paste the URI here:");
    dialog.setModal(true);
    while(true) {
        if(dialog.exec()==QDialog::Accepted) {
            if(dialog.textValue()=="") {
                advice.setText("URI field is empty!\nEnter a valid URI.");
            }
            else {
                QString uriToValidate = dialog.textValue();
                bool isValid = uriToValidate.startsWith("ftp://simulpad.archive/document#");
                if(isValid) {
                    QString docBase64 = uriToValidate.split("#").at(1);
                    QString docName = QByteArray::fromBase64(docBase64.toLocal8Bit());
                    if(this->docsList.contains(docName)) {
                        QThread* sender = new OnlineSender(SessionData::accessToSessionData().token, docName);
                        sender->start();
                        break;
                    } else {
                        advice.setText("This URI not exists or is expired!\nEnter a valid one.");
                    }
                }
                else {
                    advice.setText("URI format is not correct!\nEnter a valid URI.");
                }
            }
            advice.setIcon(QMessageBox::Critical);
            advice.exec();
        }
        else break;
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
    else if(username=="test2" && password=="test2") { //TODO: remember to remove this
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
        SessionData::accessToSessionData().userId = replyParts[0].toStdString();
        SessionData::accessToSessionData().token = replyParts[1].toStdString();
        SessionData::accessToSessionData().avatar = recoverImageFromEncodedString(replyParts[2]);
        this->docsList = replyParts[3].split("|",QString::SkipEmptyParts);

        SessionData::accessToSessionData().username = ui->loginUsernameInput->text().toStdString();
        SessionData::accessToSessionData().password = ui->loginPasswordInput->text().toStdString();
        ui->loggedUsernameLabel->setText(ui->loginUsernameInput->text());
        ui->loggedAvatar->setPixmap(SessionData::accessToSessionData().avatar);
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
        SessionData::accessToSessionData().docId = replyParts[0].toStdString();
        SessionData::accessToSessionData().isUserOnline = true;
        SessionData::accessToSessionData().isLoginCorrect = true;

        close();
    }
}

void LoginWindow::getPartecipantsResponse(QString responseString)
{
    if(responseString[0]!='0') {
        SessionData::accessToSessionData().docId = responseString.left(responseString.indexOf('|')).toStdString();
        responseString.remove(0,SessionData::accessToSessionData().docId.length()+1);

        QStringList users = responseString.split("|");
        for (QString& user : users) {
            QStringList userElements = user.split(":");

            UserTag userTag;
            userTag.setUserId(userElements[0].toInt());
            userTag.setUserColor(chooseColorFromString(userElements[1]));
            userTag.setUsername(userElements[2]);
            userTag.setAvatar(recoverImageFromEncodedString(userElements[3]));
            userTag.setUserStatus(false);

            SessionData::accessToSessionData().usersList.push_back(userTag);
            SessionData::accessToSessionData().userColorMap[userElements[0].toInt()] = userTag.getUserColor();

        }
        SessionData::accessToSessionData().isUserOnline = true;
        SessionData::accessToSessionData().isLoginCorrect = true;

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
        ui->loggedUsernameLabel->setText(QString::fromStdString(SessionData::accessToSessionData().username));
        ui->loggedAvatar->setPixmap(SessionData::accessToSessionData().avatar);
    }
    else {
        SessionData::accessToSessionData().username = ui->loggedUsernameLabel->text().toStdString();
        SessionData::accessToSessionData().avatar = *ui->loggedAvatar->pixmap();
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

void LoginWindow::mousePressEvent(QMouseEvent *event) {
    mouseClickXCoordinate = event->x();
    mouseClickYCoordinate = event->y();
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event) {
    if(event->localPos().x()<=ui->titleBar->width() && event->localPos().y()<=ui->titleBar->height()) {
        move(event->globalX()-mouseClickXCoordinate,event->globalY()-mouseClickYCoordinate);
    }
}