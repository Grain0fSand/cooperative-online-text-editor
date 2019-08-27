/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QLabel *banner;
    QFrame *formFrame;
    QLabel *loginUsernameLabel;
    QLineEdit *loginUsernameInput;
    QLineEdit *loginPasswordInput;
    QLabel *loginPasswordLabel;
    QPushButton *loginButton;
    QLabel *loginErrorLabel;
    QPushButton *loginText;
    QPushButton *registerText;
    QLabel *registerEmailLabel;
    QLineEdit *registerPasswordInput;
    QLineEdit *registerUsernameInput;
    QPushButton *registerButton;
    QLabel *registerUsernameLabel;
    QLineEdit *registerEmailInput;
    QLabel *registerPasswordLabel;
    QFrame *titleBar;
    QPushButton *closeButton;
    QPushButton *minimizeButton;
    QLabel *loginLogo;

    void setupUi(QWidget *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName(QString::fromUtf8("LoginWindow"));
        LoginWindow->resize(640, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/logoIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        LoginWindow->setWindowIcon(icon);
        banner = new QLabel(LoginWindow);
        banner->setObjectName(QString::fromUtf8("banner"));
        banner->setGeometry(QRect(0, 0, 320, 480));
        banner->setPixmap(QPixmap(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/bannerApp.png")));
        banner->setScaledContents(true);
        formFrame = new QFrame(LoginWindow);
        formFrame->setObjectName(QString::fromUtf8("formFrame"));
        formFrame->setGeometry(QRect(320, 0, 320, 480));
        formFrame->setFrameShape(QFrame::StyledPanel);
        formFrame->setFrameShadow(QFrame::Raised);
        loginUsernameLabel = new QLabel(formFrame);
        loginUsernameLabel->setObjectName(QString::fromUtf8("loginUsernameLabel"));
        loginUsernameLabel->setGeometry(QRect(60, 198, 151, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("Verdana"));
        font.setPointSize(9);
        loginUsernameLabel->setFont(font);
        loginUsernameLabel->setStyleSheet(QString::fromUtf8("padding-left: 7px;"));
        loginUsernameInput = new QLineEdit(formFrame);
        loginUsernameInput->setObjectName(QString::fromUtf8("loginUsernameInput"));
        loginUsernameInput->setGeometry(QRect(60, 220, 201, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        loginUsernameInput->setFont(font1);
        loginUsernameInput->setStyleSheet(QString::fromUtf8("border-radius: 8px;\n"
"padding-left: 8px;\n"
"background-color: white;"));
        loginUsernameInput->setEchoMode(QLineEdit::Normal);
        loginUsernameInput->setCursorPosition(0);
        loginPasswordInput = new QLineEdit(formFrame);
        loginPasswordInput->setObjectName(QString::fromUtf8("loginPasswordInput"));
        loginPasswordInput->setGeometry(QRect(60, 280, 201, 31));
        loginPasswordInput->setFont(font1);
        loginPasswordInput->setStyleSheet(QString::fromUtf8("border-radius: 8px;\n"
"padding-left: 8px;\n"
"background-color: white;"));
        loginPasswordInput->setEchoMode(QLineEdit::Password);
        loginPasswordInput->setCursorPosition(0);
        loginPasswordLabel = new QLabel(formFrame);
        loginPasswordLabel->setObjectName(QString::fromUtf8("loginPasswordLabel"));
        loginPasswordLabel->setGeometry(QRect(60, 258, 151, 21));
        loginPasswordLabel->setFont(font);
        loginPasswordLabel->setStyleSheet(QString::fromUtf8("padding-left: 7px;"));
        loginButton = new QPushButton(formFrame);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setGeometry(QRect(110, 330, 101, 31));
        loginButton->setFont(font);
        loginButton->setCursor(QCursor(Qt::PointingHandCursor));
        loginButton->setAutoFillBackground(true);
        loginErrorLabel = new QLabel(formFrame);
        loginErrorLabel->setObjectName(QString::fromUtf8("loginErrorLabel"));
        loginErrorLabel->setGeometry(QRect(60, 380, 201, 41));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Myanmar Text"));
        font2.setPointSize(9);
        loginErrorLabel->setFont(font2);
        loginErrorLabel->setStyleSheet(QString::fromUtf8("color:red"));
        loginErrorLabel->setAlignment(Qt::AlignCenter);
        loginText = new QPushButton(formFrame);
        loginText->setObjectName(QString::fromUtf8("loginText"));
        loginText->setGeometry(QRect(65, 436, 191, 24));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Myanmar Text"));
        font3.setPointSize(11);
        font3.setUnderline(true);
        loginText->setFont(font3);
        loginText->setCursor(QCursor(Qt::PointingHandCursor));
        loginText->setFocusPolicy(Qt::NoFocus);
        loginText->setStyleSheet(QString::fromUtf8("color:darkblue"));
        loginText->setFlat(true);
        registerText = new QPushButton(formFrame);
        registerText->setObjectName(QString::fromUtf8("registerText"));
        registerText->setGeometry(QRect(400, 436, 161, 24));
        registerText->setFont(font3);
        registerText->setCursor(QCursor(Qt::PointingHandCursor));
        registerText->setFocusPolicy(Qt::NoFocus);
        registerText->setStyleSheet(QString::fromUtf8("color:darkblue"));
        registerText->setFlat(true);
        registerEmailLabel = new QLabel(formFrame);
        registerEmailLabel->setObjectName(QString::fromUtf8("registerEmailLabel"));
        registerEmailLabel->setGeometry(QRect(380, 180, 151, 21));
        registerEmailLabel->setFont(font);
        registerEmailLabel->setStyleSheet(QString::fromUtf8("padding-left: 7px;"));
        registerPasswordInput = new QLineEdit(formFrame);
        registerPasswordInput->setObjectName(QString::fromUtf8("registerPasswordInput"));
        registerPasswordInput->setGeometry(QRect(380, 320, 201, 31));
        registerPasswordInput->setFont(font1);
        registerPasswordInput->setStyleSheet(QString::fromUtf8("border-radius: 8px;\n"
"padding-left: 8px;\n"
"background-color: white;"));
        registerPasswordInput->setEchoMode(QLineEdit::Password);
        registerPasswordInput->setCursorPosition(0);
        registerUsernameInput = new QLineEdit(formFrame);
        registerUsernameInput->setObjectName(QString::fromUtf8("registerUsernameInput"));
        registerUsernameInput->setGeometry(QRect(380, 260, 201, 31));
        registerUsernameInput->setFont(font1);
        registerUsernameInput->setStyleSheet(QString::fromUtf8("border-radius: 8px;\n"
"padding-left: 8px;\n"
"background-color: white;"));
        registerUsernameInput->setEchoMode(QLineEdit::Normal);
        registerUsernameInput->setCursorPosition(0);
        registerButton = new QPushButton(formFrame);
        registerButton->setObjectName(QString::fromUtf8("registerButton"));
        registerButton->setGeometry(QRect(430, 370, 101, 31));
        registerButton->setFont(font);
        registerButton->setCursor(QCursor(Qt::PointingHandCursor));
        registerButton->setAutoFillBackground(true);
        registerUsernameLabel = new QLabel(formFrame);
        registerUsernameLabel->setObjectName(QString::fromUtf8("registerUsernameLabel"));
        registerUsernameLabel->setGeometry(QRect(380, 238, 151, 21));
        registerUsernameLabel->setFont(font);
        registerUsernameLabel->setStyleSheet(QString::fromUtf8("padding-left: 7px;"));
        registerEmailInput = new QLineEdit(formFrame);
        registerEmailInput->setObjectName(QString::fromUtf8("registerEmailInput"));
        registerEmailInput->setGeometry(QRect(380, 202, 201, 31));
        registerEmailInput->setFont(font1);
        registerEmailInput->setStyleSheet(QString::fromUtf8("border-radius: 8px;\n"
"padding-left: 8px;\n"
"background-color: white;"));
        registerEmailInput->setEchoMode(QLineEdit::Normal);
        registerEmailInput->setCursorPosition(0);
        registerPasswordLabel = new QLabel(formFrame);
        registerPasswordLabel->setObjectName(QString::fromUtf8("registerPasswordLabel"));
        registerPasswordLabel->setGeometry(QRect(380, 298, 151, 21));
        registerPasswordLabel->setFont(font);
        registerPasswordLabel->setStyleSheet(QString::fromUtf8("padding-left: 7px;"));
        loginUsernameLabel->raise();
        loginUsernameInput->raise();
        loginPasswordInput->raise();
        loginPasswordLabel->raise();
        loginButton->raise();
        loginErrorLabel->raise();
        loginText->raise();
        registerText->raise();
        registerEmailLabel->raise();
        registerPasswordInput->raise();
        registerButton->raise();
        registerUsernameInput->raise();
        registerUsernameLabel->raise();
        registerEmailInput->raise();
        registerPasswordLabel->raise();
        titleBar = new QFrame(LoginWindow);
        titleBar->setObjectName(QString::fromUtf8("titleBar"));
        titleBar->setGeometry(QRect(0, 0, 640, 30));
        titleBar->setFrameShape(QFrame::StyledPanel);
        titleBar->setFrameShadow(QFrame::Raised);
        closeButton = new QPushButton(titleBar);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(610, 5, 21, 23));
        closeButton->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/closeButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        closeButton->setIcon(icon1);
        closeButton->setFlat(true);
        minimizeButton = new QPushButton(titleBar);
        minimizeButton->setObjectName(QString::fromUtf8("minimizeButton"));
        minimizeButton->setGeometry(QRect(589, 5, 21, 23));
        minimizeButton->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/minimizeButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        minimizeButton->setIcon(icon2);
        minimizeButton->setFlat(true);
        loginLogo = new QLabel(LoginWindow);
        loginLogo->setObjectName(QString::fromUtf8("loginLogo"));
        loginLogo->setGeometry(QRect(420, 45, 120, 120));
        loginLogo->setPixmap(QPixmap(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/logoApp.png")));
        loginLogo->setScaledContents(true);
        formFrame->raise();
        banner->raise();
        titleBar->raise();
        loginLogo->raise();

        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QWidget *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "Login", nullptr));
        banner->setText(QString());
        loginUsernameLabel->setText(QCoreApplication::translate("LoginWindow", "Username", nullptr));
        loginUsernameInput->setText(QString());
        loginPasswordInput->setText(QString());
        loginPasswordLabel->setText(QCoreApplication::translate("LoginWindow", "Password", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginWindow", "Login", nullptr));
        loginErrorLabel->setText(QString());
        loginText->setText(QCoreApplication::translate("LoginWindow", "Register a new account \342\200\225>", nullptr));
        registerText->setText(QCoreApplication::translate("LoginWindow", "<\342\200\225 Back to login page", nullptr));
        registerEmailLabel->setText(QCoreApplication::translate("LoginWindow", "Email address", nullptr));
        registerPasswordInput->setText(QString());
        registerUsernameInput->setText(QString());
        registerButton->setText(QCoreApplication::translate("LoginWindow", "Register", nullptr));
        registerUsernameLabel->setText(QCoreApplication::translate("LoginWindow", "Username", nullptr));
        registerEmailInput->setText(QString());
        registerPasswordLabel->setText(QCoreApplication::translate("LoginWindow", "Password", nullptr));
        closeButton->setText(QString());
        minimizeButton->setText(QString());
        loginLogo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
