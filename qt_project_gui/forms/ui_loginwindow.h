/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QAction *actionChangeUsername;
    QAction *actionChangePassword;
    QAction *actionChangeAvatar;
    QLabel *banner;
    QFrame *formFrame;
    QLabel *loginUsernameLabel;
    QLineEdit *loginUsernameInput;
    QLabel *loginPasswordLabel;
    QLineEdit *loginPasswordInput;
    QPushButton *loginButton;
    QLabel *loginErrorLabel;
    QPushButton *loginText;
    QPushButton *registerText;
    QLabel *registerEmailLabel;
    QLineEdit *registerEmailInput;
    QLabel *registerUsernameLabel;
    QLineEdit *registerUsernameInput;
    QLabel *registerPasswordLabel;
    QLineEdit *registerPasswordInput;
    QPushButton *registerButton;
    QLabel *registerErrorLabel;
    QPushButton *loggedNewButton;
    QPushButton *loggedOpenButton;
    QPushButton *loggedLogoutButton;
    QToolButton *loggedSettingsButton;
    QLabel *loggedWelcomeLabel;
    QLabel *loggedAvatar;
    QLabel *loggedUsernameLabel;
    QPushButton *loggedURIButton;
    QFrame *informationWhiteFrame;
    QLabel *informationLabel;
    QPushButton *informationButton;
    QLabel *informationImage;
    QFrame *titleBar;
    QPushButton *closeButton;
    QPushButton *minimizeButton;
    QLabel *formLogo;
    QFrame *verticalSeparator;

    void setupUi(QWidget *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName(QString::fromUtf8("LoginWindow"));
        LoginWindow->resize(640, 480);
        LoginWindow->setStyleSheet("background-color:#F2ECFF;"
                                   "QLineEdit { background: white; }");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/logoIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        LoginWindow->setWindowIcon(icon);
        LoginWindow->setAutoFillBackground(false);
        actionChangeUsername = new QAction(LoginWindow);
        actionChangeUsername->setObjectName(QString::fromUtf8("actionChangeUsername"));
        actionChangePassword = new QAction(LoginWindow);
        actionChangePassword->setObjectName(QString::fromUtf8("actionChangePassword"));
        actionChangeAvatar = new QAction(LoginWindow);
        actionChangeAvatar->setObjectName(QString::fromUtf8("actionChangeAvatar"));
        banner = new QLabel(LoginWindow);
        banner->setObjectName(QString::fromUtf8("banner"));
        banner->setGeometry(QRect(0, 0, 320, 480));
        banner->setPixmap(QPixmap(QString::fromUtf8(":/resources/bannerApp.png")));
        banner->setScaledContents(true);
        formFrame = new QFrame(LoginWindow);
        formFrame->setObjectName(QString::fromUtf8("formFrame"));
        formFrame->setGeometry(QRect(0, 0, 640, 480));
        loginUsernameLabel = new QLabel(formFrame);
        loginUsernameLabel->setObjectName(QString::fromUtf8("loginUsernameLabel"));
        loginUsernameLabel->setGeometry(QRect(380, 198, 151, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("Verdana"));
        font.setPointSize(9);
        loginUsernameLabel->setFont(font);
        loginUsernameLabel->setStyleSheet(QString::fromUtf8("padding-left: 7px;"));
        loginUsernameInput = new QLineEdit(formFrame);
        loginUsernameInput->setObjectName(QString::fromUtf8("loginUsernameInput"));
        loginUsernameInput->setGeometry(QRect(380, 220, 201, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        loginUsernameInput->setFont(font1);
        loginUsernameInput->setStyleSheet(QString::fromUtf8("border-radius: 8px;\n"
"padding-left: 8px;\n"
"background-color: white;"));
        loginUsernameInput->setEchoMode(QLineEdit::Normal);
        loginUsernameInput->setCursorPosition(0);
        loginPasswordLabel = new QLabel(formFrame);
        loginPasswordLabel->setObjectName(QString::fromUtf8("loginPasswordLabel"));
        loginPasswordLabel->setGeometry(QRect(380, 258, 151, 21));
        loginPasswordLabel->setFont(font);
        loginPasswordLabel->setStyleSheet(QString::fromUtf8("padding-left: 7px;"));
        loginPasswordInput = new QLineEdit(formFrame);
        loginPasswordInput->setObjectName(QString::fromUtf8("loginPasswordInput"));
        loginPasswordInput->setGeometry(QRect(380, 280, 201, 31));
        loginPasswordInput->setFont(font1);
        loginPasswordInput->setStyleSheet(QString::fromUtf8("border-radius: 8px;\n"
"padding-left: 8px;\n"
"background-color: white;"));
        loginPasswordInput->setEchoMode(QLineEdit::Password);
        loginPasswordInput->setCursorPosition(0);
        loginButton = new QPushButton(formFrame);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setGeometry(QRect(430, 330, 101, 31));
        loginButton->setFont(font);
        loginButton->setCursor(QCursor(Qt::PointingHandCursor));
        loginButton->setAutoFillBackground(false);
        loginButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: #E1E1E1;\n"
"border-radius: 8;\n"
"padding: 3px;\n"
"padding-left: 5px;\n"
"padding-right: 5px;\n"
"border-width: 1px;\n"
"border-color: #ADADAD;\n"
"border-style: solid;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"background-color: #E4EFF9;\n"
"border-color: #0C7BD4;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"background-color: #CDE4F7;\n"
"border-color: #00559B;\n"
"}"));
        loginErrorLabel = new QLabel(formFrame);
        loginErrorLabel->setObjectName(QString::fromUtf8("loginErrorLabel"));
        loginErrorLabel->setGeometry(QRect(380, 380, 201, 41));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Myanmar Text"));
        font2.setPointSize(9);
        loginErrorLabel->setFont(font2);
        loginErrorLabel->setStyleSheet(QString::fromUtf8("color:red"));
        loginErrorLabel->setAlignment(Qt::AlignCenter);
        loginText = new QPushButton(formFrame);
        loginText->setObjectName(QString::fromUtf8("loginText"));
        loginText->setGeometry(QRect(375, 436, 211, 24));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Myanmar Text"));
        font3.setPointSize(11);
        font3.setUnderline(true);
        loginText->setFont(font3);
        loginText->setCursor(QCursor(Qt::PointingHandCursor));
        loginText->setFocusPolicy(Qt::NoFocus);
        loginText->setStyleSheet(QString::fromUtf8("color:#3C505B"));
        loginText->setFlat(true);
        registerText = new QPushButton(formFrame);
        registerText->setObjectName(QString::fromUtf8("registerText"));
        registerText->setGeometry(QRect(720, 436, 161, 24));
        registerText->setFont(font3);
        registerText->setCursor(QCursor(Qt::PointingHandCursor));
        registerText->setFocusPolicy(Qt::NoFocus);
        registerText->setStyleSheet(QString::fromUtf8("color:#3C505B"));
        registerText->setFlat(true);
        registerEmailLabel = new QLabel(formFrame);
        registerEmailLabel->setObjectName(QString::fromUtf8("registerEmailLabel"));
        registerEmailLabel->setGeometry(QRect(700, 173, 151, 21));
        registerEmailLabel->setFont(font);
        registerEmailLabel->setStyleSheet(QString::fromUtf8("padding-left: 7px;"));
        registerEmailInput = new QLineEdit(formFrame);
        registerEmailInput->setObjectName(QString::fromUtf8("registerEmailInput"));
        registerEmailInput->setGeometry(QRect(700, 195, 201, 31));
        registerEmailInput->setFont(font1);
        registerEmailInput->setStyleSheet(QString::fromUtf8("border-radius: 8px;\n"
"padding-left: 8px;\n"
"background-color: white;"));
        registerEmailInput->setEchoMode(QLineEdit::Normal);
        registerEmailInput->setCursorPosition(0);
        registerUsernameLabel = new QLabel(formFrame);
        registerUsernameLabel->setObjectName(QString::fromUtf8("registerUsernameLabel"));
        registerUsernameLabel->setGeometry(QRect(700, 228, 151, 21));
        registerUsernameLabel->setFont(font);
        registerUsernameLabel->setStyleSheet(QString::fromUtf8("padding-left: 7px;"));
        registerUsernameInput = new QLineEdit(formFrame);
        registerUsernameInput->setObjectName(QString::fromUtf8("registerUsernameInput"));
        registerUsernameInput->setGeometry(QRect(700, 250, 201, 31));
        registerUsernameInput->setFont(font1);
        registerUsernameInput->setStyleSheet(QString::fromUtf8("border-radius: 8px;\n"
"padding-left: 8px;\n"
"background-color: white;"));
        registerUsernameInput->setEchoMode(QLineEdit::Normal);
        registerUsernameInput->setCursorPosition(0);
        registerPasswordLabel = new QLabel(formFrame);
        registerPasswordLabel->setObjectName(QString::fromUtf8("registerPasswordLabel"));
        registerPasswordLabel->setGeometry(QRect(700, 283, 151, 21));
        registerPasswordLabel->setFont(font);
        registerPasswordLabel->setStyleSheet(QString::fromUtf8("padding-left: 7px;"));
        registerPasswordInput = new QLineEdit(formFrame);
        registerPasswordInput->setObjectName(QString::fromUtf8("registerPasswordInput"));
        registerPasswordInput->setGeometry(QRect(700, 305, 201, 31));
        registerPasswordInput->setFont(font1);
        registerPasswordInput->setStyleSheet(QString::fromUtf8("border-radius: 8px;\n"
"padding-left: 8px;\n"
"background-color: white;"));
        registerPasswordInput->setEchoMode(QLineEdit::Password);
        registerPasswordInput->setCursorPosition(0);
        registerButton = new QPushButton(formFrame);
        registerButton->setObjectName(QString::fromUtf8("registerButton"));
        registerButton->setGeometry(QRect(750, 350, 101, 31));
        registerButton->setFont(font);
        registerButton->setCursor(QCursor(Qt::PointingHandCursor));
        registerButton->setAutoFillBackground(false);
        registerButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: #E1E1E1;\n"
"border-radius: 8;\n"
"padding: 3px;\n"
"padding-left: 5px;\n"
"padding-right: 5px;\n"
"border-width: 1px;\n"
"border-color: #ADADAD;\n"
"border-style: solid;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"background-color: #E4EFF9;\n"
"border-color: #0C7BD4;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"background-color: #CDE4F7;\n"
"border-color: #00559B;\n"
"}"));
        registerErrorLabel = new QLabel(formFrame);
        registerErrorLabel->setObjectName(QString::fromUtf8("registerErrorLabel"));
        registerErrorLabel->setGeometry(QRect(700, 390, 201, 41));
        registerErrorLabel->setFont(font2);
        registerErrorLabel->setStyleSheet(QString::fromUtf8("color:red"));
        registerErrorLabel->setAlignment(Qt::AlignCenter);
        loggedNewButton = new QPushButton(formFrame);
        loggedNewButton->setObjectName(QString::fromUtf8("loggedNewButton"));
        loggedNewButton->setGeometry(QRect(70, 280, 181, 42));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Verdana"));
        font4.setPointSize(11);
        loggedNewButton->setFont(font4);
        loggedNewButton->setCursor(QCursor(Qt::PointingHandCursor));
        loggedNewButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	text-align: left;\n"
"}\n"
"QPushButton:hover {\n"
"	text-align: left;\n"
"	font-size: 16px;\n"
"}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/newDoc.png"), QSize(), QIcon::Normal, QIcon::Off);
        loggedNewButton->setIcon(icon1);
        loggedNewButton->setIconSize(QSize(40, 40));
        loggedNewButton->setFlat(true);
        loggedOpenButton = new QPushButton(formFrame);
        loggedOpenButton->setObjectName(QString::fromUtf8("loggedOpenButton"));
        loggedOpenButton->setGeometry(QRect(70, 320, 191, 42));
        loggedOpenButton->setFont(font4);
        loggedOpenButton->setCursor(QCursor(Qt::PointingHandCursor));
        loggedOpenButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	text-align: left;\n"
"}\n"
"QPushButton:hover {\n"
"	text-align: left;\n"
"	font-size: 16px;\n"
"}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resources/openDoc.png"), QSize(), QIcon::Normal, QIcon::Off);
        loggedOpenButton->setIcon(icon2);
        loggedOpenButton->setIconSize(QSize(40, 40));
        loggedOpenButton->setFlat(true);
        loggedLogoutButton = new QPushButton(formFrame);
        loggedLogoutButton->setObjectName(QString::fromUtf8("loggedLogoutButton"));
        loggedLogoutButton->setGeometry(QRect(76, 400, 111, 42));
        loggedLogoutButton->setFont(font4);
        loggedLogoutButton->setCursor(QCursor(Qt::PointingHandCursor));
        loggedLogoutButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	text-align: left;\n"
"}\n"
"QPushButton:hover {\n"
"	text-align: left;\n"
"	font-size: 16px;\n"
"}"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resources/logOut.png"), QSize(), QIcon::Normal, QIcon::Off);
        loggedLogoutButton->setIcon(icon3);
        loggedLogoutButton->setIconSize(QSize(40, 40));
        loggedLogoutButton->setFlat(true);
        loggedSettingsButton = new QToolButton(formFrame);
        loggedSettingsButton->setObjectName(QString::fromUtf8("loggedSettingsButton"));
        loggedSettingsButton->setGeometry(QRect(245, 190, 36, 21));
        loggedSettingsButton->setStyleSheet(QString::fromUtf8("color: #707070;"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resources/ic_settings_18pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        loggedSettingsButton->setIcon(icon4);
        loggedSettingsButton->setIconSize(QSize(20, 20));
        loggedSettingsButton->setPopupMode(QToolButton::InstantPopup);
        loggedSettingsButton->setAutoRaise(true);
        loggedWelcomeLabel = new QLabel(formFrame);
        loggedWelcomeLabel->setObjectName(QString::fromUtf8("loggedWelcomeLabel"));
        loggedWelcomeLabel->setGeometry(QRect(110, 190, 71, 21));
        loggedWelcomeLabel->setFont(font4);
        loggedAvatar = new QLabel(formFrame);
        loggedAvatar->setObjectName(QString::fromUtf8("loggedAvatar"));
        loggedAvatar->setGeometry(QRect(40, 190, 60, 60));
        loggedAvatar->setStyleSheet(QString::fromUtf8(""));
        loggedAvatar->setFrameShape(QFrame::Panel);
        loggedAvatar->setFrameShadow(QFrame::Raised);
        loggedAvatar->setLineWidth(1);
        loggedAvatar->setPixmap(QPixmap(QString::fromUtf8(":/resources/avatar.png")));
        loggedAvatar->setScaledContents(true);
        loggedUsernameLabel = new QLabel(formFrame);
        loggedUsernameLabel->setObjectName(QString::fromUtf8("loggedUsernameLabel"));
        loggedUsernameLabel->setGeometry(QRect(110, 212, 151, 39));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Verdana"));
        font5.setPointSize(14);
        loggedUsernameLabel->setFont(font5);
        loggedURIButton = new QPushButton(formFrame);
        loggedURIButton->setObjectName(QString::fromUtf8("loggedURIButton"));
        loggedURIButton->setGeometry(QRect(70, 360, 186, 42));
        loggedURIButton->setFont(font4);
        loggedURIButton->setCursor(QCursor(Qt::PointingHandCursor));
        loggedURIButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	text-align: left;\n"
"}\n"
"QPushButton:hover {\n"
"	text-align: left;\n"
"	font-size: 16px;\n"
"}"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/resources/openURI.png"), QSize(), QIcon::Normal, QIcon::Off);
        loggedURIButton->setIcon(icon5);
        loggedURIButton->setIconSize(QSize(40, 40));
        loggedURIButton->setFlat(true);
        informationWhiteFrame = new QFrame(formFrame);
        informationWhiteFrame->setObjectName(QString::fromUtf8("informationWhiteFrame"));
        informationWhiteFrame->setGeometry(QRect(640, 0, 320, 480));
        informationWhiteFrame->setStyleSheet(QString::fromUtf8("background: #F2ECFF;"));
        informationWhiteFrame->setFrameShape(QFrame::NoFrame);
        informationWhiteFrame->setFrameShadow(QFrame::Plain);
        informationLabel = new QLabel(informationWhiteFrame);
        informationLabel->setObjectName(QString::fromUtf8("informationLabel"));
        informationLabel->setGeometry(QRect(5, 200, 310, 100));
        QFont font6;
        font6.setFamily(QString::fromUtf8("Gill Sans MT"));
        font6.setPointSize(12);
        font6.setBold(true);
        font6.setWeight(75);
        informationLabel->setFont(font6);
        informationLabel->setAlignment(Qt::AlignCenter);
        informationButton = new QPushButton(informationWhiteFrame);
        informationButton->setObjectName(QString::fromUtf8("informationButton"));
        informationButton->setGeometry(QRect(110, 350, 101, 31));
        informationButton->setFont(font);
        informationButton->setCursor(QCursor(Qt::PointingHandCursor));
        informationButton->setAutoFillBackground(false);
        informationButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: #E1E1E1;\n"
"border-radius: 8;\n"
"padding: 3px;\n"
"padding-left: 5px;\n"
"padding-right: 5px;\n"
"border-width: 1px;\n"
"border-color: #ADADAD;\n"
"border-style: solid;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"background-color: #E4EFF9;\n"
"border-color: #0C7BD4;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"background-color: #CDE4F7;\n"
"border-color: #00559B;\n"
"}"));
        informationImage = new QLabel(informationWhiteFrame);
        informationImage->setObjectName(QString::fromUtf8("informationImage"));
        informationImage->setGeometry(QRect(110, 270, 100, 100));
        informationImage->setScaledContents(true);
        loginUsernameLabel->raise();
        loginUsernameInput->raise();
        loginPasswordLabel->raise();
        loginPasswordInput->raise();
        loginButton->raise();
        loginErrorLabel->raise();
        loginText->raise();
        registerText->raise();
        registerEmailLabel->raise();
        registerEmailInput->raise();
        registerUsernameLabel->raise();
        registerUsernameInput->raise();
        registerPasswordLabel->raise();
        registerPasswordInput->raise();
        registerButton->raise();
        registerErrorLabel->raise();
        loggedNewButton->raise();
        loggedOpenButton->raise();
        loggedLogoutButton->raise();
        loggedSettingsButton->raise();
        loggedWelcomeLabel->raise();
        loggedAvatar->raise();
        loggedUsernameLabel->raise();
        loggedURIButton->raise();
        informationWhiteFrame->raise();
        titleBar = new QFrame(LoginWindow);
        titleBar->setObjectName(QString::fromUtf8("titleBar"));
        titleBar->setGeometry(QRect(0, 0, 640, 30));
        closeButton = new QPushButton(titleBar);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(610, 5, 21, 23));
        closeButton->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/resources/closeButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        closeButton->setIcon(icon6);
        closeButton->setFlat(true);
        minimizeButton = new QPushButton(titleBar);
        minimizeButton->setObjectName(QString::fromUtf8("minimizeButton"));
        minimizeButton->setGeometry(QRect(589, 5, 21, 23));
        minimizeButton->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/resources/minimizeButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        minimizeButton->setIcon(icon7);
        minimizeButton->setFlat(true);
        formLogo = new QLabel(LoginWindow);
        formLogo->setObjectName(QString::fromUtf8("formLogo"));
        formLogo->setGeometry(QRect(420, 45, 120, 120));
        formLogo->setPixmap(QPixmap(QString::fromUtf8(":/resources/logoApp.png")));
        formLogo->setScaledContents(true);
        verticalSeparator = new QFrame(LoginWindow);
        verticalSeparator->setObjectName(QString::fromUtf8("verticalSeparator"));
        verticalSeparator->setGeometry(QRect(320, -1, 1, 482));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(verticalSeparator->sizePolicy().hasHeightForWidth());
        verticalSeparator->setSizePolicy(sizePolicy);
        verticalSeparator->setFrameShadow(QFrame::Raised);
        verticalSeparator->setFrameShape(QFrame::VLine);
        formFrame->raise();
        titleBar->raise();
        formLogo->raise();
        banner->raise();
        verticalSeparator->raise();

        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QWidget *LoginWindow)
    {
        actionChangeUsername->setText(QCoreApplication::translate("LoginWindow", "Change your username", nullptr));
        actionChangePassword->setText(QCoreApplication::translate("LoginWindow", "Change your password", nullptr));
        actionChangeAvatar->setText(QCoreApplication::translate("LoginWindow", "Change your avatar", nullptr));
        banner->setText(QString());
        loginUsernameLabel->setText(QCoreApplication::translate("LoginWindow", "Username", nullptr));
        loginUsernameInput->setText(QString());
        loginPasswordLabel->setText(QCoreApplication::translate("LoginWindow", "Password", nullptr));
        loginPasswordInput->setText(QString());
        loginButton->setText(QCoreApplication::translate("LoginWindow", "Login", nullptr));
        loginErrorLabel->setText(QString());
        loginText->setText(QCoreApplication::translate("LoginWindow", "Register a new account \342\200\225>", nullptr));
        registerText->setText(QCoreApplication::translate("LoginWindow", "<\342\200\225 Back to login page", nullptr));
        registerEmailLabel->setText(QCoreApplication::translate("LoginWindow", "Email address", nullptr));
        registerEmailInput->setText(QString());
        registerUsernameLabel->setText(QCoreApplication::translate("LoginWindow", "Username", nullptr));
        registerUsernameInput->setText(QString());
        registerPasswordLabel->setText(QCoreApplication::translate("LoginWindow", "Password", nullptr));
        registerPasswordInput->setText(QString());
        registerButton->setText(QCoreApplication::translate("LoginWindow", "Register", nullptr));
        registerErrorLabel->setText(QString());
        loggedNewButton->setText(QCoreApplication::translate("LoginWindow", " New Document", nullptr));
        loggedOpenButton->setText(QCoreApplication::translate("LoginWindow", " Open Document", nullptr));
        loggedLogoutButton->setText(QCoreApplication::translate("LoginWindow", "Logout", nullptr));
        loggedSettingsButton->setText(QString());
        loggedWelcomeLabel->setText(QCoreApplication::translate("LoginWindow", "Welcome", nullptr));
        loggedAvatar->setText(QString());
        loggedURIButton->setText(QCoreApplication::translate("LoginWindow", " Open From URI", nullptr));
        informationLabel->setText(QCoreApplication::translate("LoginWindow", "", nullptr));
        informationButton->setText(QCoreApplication::translate("LoginWindow", "", nullptr));
        informationImage->setText(QString());
        closeButton->setText(QString());
        minimizeButton->setText(QString());
        formLogo->setText(QString());
        Q_UNUSED(LoginWindow);
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
