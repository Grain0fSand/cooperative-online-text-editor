#include "loginwindow.h"
#include "ui_loginwindow.h"
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
    connect(ui->loggedLogoutButton,&QPushButton::clicked,this,&LoginWindow::switchFrame);
    connect(ui->actionChangeUsername,&QAction::triggered,this,&LoginWindow::changeYourUsername);
    connect(ui->actionChangeAvatar,&QAction::triggered,this,&LoginWindow::changeYourAvatar);
    connect(ui->loggedNewButton,&QPushButton::clicked,this,&LoginWindow::openEditor);
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
    a->start();

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

    switchAnimation->start();
}

void LoginWindow::changeYourUsername()
{
    QInputDialog* dialog = new QInputDialog(this);
    dialog->setLabelText("Change your username:");
    dialog->setTextValue(ui->loggedUsernameLabel->text());
    dialog->setModal(true);
    while(true) {
        if(dialog->exec()==QDialog::Accepted && dialog->textValue()=="") {
            QMessageBox* advice = new QMessageBox(this);
            advice->setText("Username field empty!\nEnter a valid username");
            advice->setIcon(QMessageBox::Critical);
            advice->exec();
        }
        else {
            ui->loggedUsernameLabel->setText(dialog->textValue());
            break;
        }
    }
}

void LoginWindow::changeYourAvatar()
{
    QString file_path = QFileDialog::getOpenFileName(nullptr,"Choose your own avatar..");
    if(file_path!="") {
        QString file_name = file_path.split("/").last();
        QString new_file_path = "://PdsProject.app/Contents/Resources/avatars/"+file_name;
        QFile::copy(file_path,new_file_path);

        ui->loggedAvatar->setPixmap(QPixmap(new_file_path));
    }
}

void LoginWindow::openEditor()
{
    this->loginCorrect=true;
    close();
}

void LoginWindow::requestURI()
{
    QInputDialog* dialog = new QInputDialog(this);
    dialog->setLabelText("Paste the URI here:");
    dialog->setModal(true);
    while(true) {
        if(dialog->exec()==QDialog::Accepted && dialog->textValue()=="") {
            QMessageBox* advice = new QMessageBox(this);
            advice->setText("URI field empty!\nEnter a valid URI");
            advice->setIcon(QMessageBox::Critical);
            advice->exec();
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
        ui->loginErrorLabel->setText("Login failed");
    }
    else {
        ui->loggedUsernameLabel->setText(ui->loginUsernameInput->text());
        this->switchFrame(1);
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
    }
}
