/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "mytextedit.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionCopy;
    QAction *actionCut;
    QAction *actionPaste;
    QAction *actionBold;
    QAction *actionItalic;
    QAction *actionUnderlined;
    QAction *actionAlignLeft;
    QAction *actionAlignCenter;
    QAction *actionAlignRight;
    QAction *actionConnect;
    QAction *actionExport_to_PDF;
    QAction *actionExit;
    QAction *actionAlignJustify;
    QAction *actionInvite;
    QAction *actionTestCursor;
    QAction *actionTestDisconnect;
    QAction *actionTestColor;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QWidget *docFrame;
    QGridLayout *docFrameLayout;
    MyTextEdit *textEditShared;
    QFrame *verticalLine;
    QVBoxLayout *sideLayout;
    QFrame *myTag;
    QLabel *myUsername;
    QLabel *myStatus;
    QLabel *myAvatar;
    QLabel *myLed;
    QPushButton *onlineRollButton;
    QListWidget *listOnlineUsers;
    QPushButton *offlineRollButton;
    QListWidget *listOfflineUsers;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuMain;
    QMenu *menuAbout;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1034, 691);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1034, 691));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/logoIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/ic_content_copy_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon1);
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resources/ic_content_cut_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon2);
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resources/ic_content_paste_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon3);
        actionBold = new QAction(MainWindow);
        actionBold->setObjectName(QString::fromUtf8("actionBold"));
        actionBold->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resources/ic_format_bold_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBold->setIcon(icon4);
        actionItalic = new QAction(MainWindow);
        actionItalic->setObjectName(QString::fromUtf8("actionItalic"));
        actionItalic->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/resources/ic_format_italic_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionItalic->setIcon(icon5);
        actionUnderlined = new QAction(MainWindow);
        actionUnderlined->setObjectName(QString::fromUtf8("actionUnderlined"));
        actionUnderlined->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/resources/ic_format_underlined_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUnderlined->setIcon(icon6);
        actionAlignLeft = new QAction(MainWindow);
        actionAlignLeft->setObjectName(QString::fromUtf8("actionAlignLeft"));
        actionAlignLeft->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/resources/ic_format_align_left_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAlignLeft->setIcon(icon7);
        actionAlignCenter = new QAction(MainWindow);
        actionAlignCenter->setObjectName(QString::fromUtf8("actionAlignCenter"));
        actionAlignCenter->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/resources/ic_format_align_center_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAlignCenter->setIcon(icon8);
        actionAlignRight = new QAction(MainWindow);
        actionAlignRight->setObjectName(QString::fromUtf8("actionAlignRight"));
        actionAlignRight->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/resources/ic_format_align_right_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAlignRight->setIcon(icon9);
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionExport_to_PDF = new QAction(MainWindow);
        actionExport_to_PDF->setObjectName(QString::fromUtf8("actionExport_to_PDF"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAlignJustify = new QAction(MainWindow);
        actionAlignJustify->setObjectName(QString::fromUtf8("actionAlignJustify"));
        actionAlignJustify->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/resources/ic_format_align_justify_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAlignJustify->setIcon(icon10);
        actionInvite = new QAction(MainWindow);
        actionInvite->setObjectName(QString::fromUtf8("actionInvite"));
        actionTestCursor = new QAction(MainWindow);
        actionTestCursor->setObjectName(QString::fromUtf8("actionTestCursor"));
        actionTestDisconnect = new QAction(MainWindow);
        actionTestDisconnect->setObjectName(QString::fromUtf8("actionTestDisconnect"));
        actionTestColor = new QAction(MainWindow);
        actionTestColor->setObjectName(QString::fromUtf8("actionTestColor"));
        actionTestColor->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/resources/colorButtonIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTestColor->setIcon(icon11);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMinimumSize(QSize(1000, 600));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        docFrame = new QWidget(centralWidget);
        docFrame->setObjectName(QString::fromUtf8("docFrame"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(docFrame->sizePolicy().hasHeightForWidth());
        docFrame->setSizePolicy(sizePolicy1);
        docFrame->setMinimumSize(QSize(750, 550));
        docFrameLayout = new QGridLayout(docFrame);
        docFrameLayout->setSpacing(6);
        docFrameLayout->setContentsMargins(11, 11, 11, 11);
        docFrameLayout->setObjectName(QString::fromUtf8("docFrameLayout"));
        docFrameLayout->setContentsMargins(0, 0, 0, 0);
        textEditShared = &MyTextEdit::getInstance();
        textEditShared->setObjectName(QString::fromUtf8("textEditShared"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(textEditShared->sizePolicy().hasHeightForWidth());
        textEditShared->setSizePolicy(sizePolicy2);
        textEditShared->setMinimumSize(QSize(750, 550));
        QFont font;
        font.setFamily(QString::fromUtf8("Calibri"));
        font.setPointSize(11);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        textEditShared->setFont(font);

        docFrameLayout->addWidget(textEditShared, 0, 1, 1, 1);


        horizontalLayout->addWidget(docFrame, 0, Qt::AlignHCenter);

        verticalLine = new QFrame(centralWidget);
        verticalLine->setObjectName(QString::fromUtf8("verticalLine"));
        verticalLine->setFrameShadow(QFrame::Raised);
        verticalLine->setFrameShape(QFrame::VLine);

        horizontalLayout->addWidget(verticalLine);

        sideLayout = new QVBoxLayout();
        sideLayout->setSpacing(6);
        sideLayout->setObjectName(QString::fromUtf8("sideLayout"));
        myTag = new QFrame(centralWidget);
        myTag->setObjectName(QString::fromUtf8("myTag"));
        sizePolicy.setHeightForWidth(myTag->sizePolicy().hasHeightForWidth());
        myTag->setSizePolicy(sizePolicy);
        myTag->setMinimumSize(QSize(258, 92));
        myTag->setMaximumSize(QSize(258, 92));
        QFont font1;
        font1.setPointSize(8);
        myTag->setFont(font1);
        myTag->setFrameShape(QFrame::WinPanel);
        myTag->setFrameShadow(QFrame::Raised);
        myUsername = new QLabel(myTag);
        myUsername->setObjectName(QString::fromUtf8("myUsername"));
        myUsername->setGeometry(QRect(90, 10, 161, 41));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Gill Sans MT"));
        font2.setPointSize(17);
        myUsername->setFont(font2);
        myStatus = new QLabel(myTag);
        myStatus->setObjectName(QString::fromUtf8("myStatus"));
        myStatus->setGeometry(QRect(120, 60, 101, 21));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Verdana"));
        font3.setPointSize(11);
        myStatus->setFont(font3);
        myAvatar = new QLabel(myTag);
        myAvatar->setObjectName(QString::fromUtf8("myAvatar"));
        myAvatar->setGeometry(QRect(10, 10, 71, 71));
        myAvatar->setPixmap(QPixmap(QString::fromUtf8(":/resources/avatar.png")));
        myAvatar->setScaledContents(true);
        myLed = new QLabel(myTag);
        myLed->setObjectName(QString::fromUtf8("myLed"));
        myLed->setGeometry(QRect(90, 60, 21, 21));
        myLed->setPixmap(QPixmap(QString::fromUtf8(":/resources/greenLed.png")));
        myLed->setScaledContents(true);

        sideLayout->addWidget(myTag);

        onlineRollButton = new QPushButton(centralWidget);
        onlineRollButton->setObjectName(QString::fromUtf8("onlineRollButton"));
        sizePolicy.setHeightForWidth(onlineRollButton->sizePolicy().hasHeightForWidth());
        onlineRollButton->setSizePolicy(sizePolicy);
        onlineRollButton->setMinimumSize(QSize(258, 0));
        onlineRollButton->setMaximumSize(QSize(258, 16777215));
        onlineRollButton->setLayoutDirection(Qt::RightToLeft);
        onlineRollButton->setStyleSheet(QString::fromUtf8("text-align:left;"));
        QIcon icon12;
        icon11.addFile(QString::fromUtf8(":/resources/arrow_up.png"), QSize(), QIcon::Normal, QIcon::Off);
        onlineRollButton->setIcon(icon11);
        onlineRollButton->setIconSize(QSize(10, 10));

        sideLayout->addWidget(onlineRollButton);

        listOnlineUsers = new QListWidget(centralWidget);
        listOnlineUsers->setObjectName(QString::fromUtf8("listOnlineUsers"));
        sizePolicy2.setHeightForWidth(listOnlineUsers->sizePolicy().hasHeightForWidth());
        listOnlineUsers->setSizePolicy(sizePolicy2);
        listOnlineUsers->setMinimumSize(QSize(258, 0));
        listOnlineUsers->setMaximumSize(QSize(258, 16777215));

        sideLayout->addWidget(listOnlineUsers);

        offlineRollButton = new QPushButton(centralWidget);
        offlineRollButton->setObjectName(QString::fromUtf8("offlineRollButton"));
        sizePolicy.setHeightForWidth(offlineRollButton->sizePolicy().hasHeightForWidth());
        offlineRollButton->setSizePolicy(sizePolicy);
        offlineRollButton->setMinimumSize(QSize(258, 0));
        offlineRollButton->setMaximumSize(QSize(258, 16777215));
        offlineRollButton->setLayoutDirection(Qt::RightToLeft);
        offlineRollButton->setStyleSheet(QString::fromUtf8("text-align:left;"));
        offlineRollButton->setIcon(icon11);
        offlineRollButton->setIconSize(QSize(10, 10));

        sideLayout->addWidget(offlineRollButton);

        listOfflineUsers = new QListWidget(centralWidget);
        listOfflineUsers->setObjectName(QString::fromUtf8("listOfflineUsers"));
        sizePolicy2.setHeightForWidth(listOfflineUsers->sizePolicy().hasHeightForWidth());
        listOfflineUsers->setSizePolicy(sizePolicy2);
        listOfflineUsers->setMinimumSize(QSize(258, 0));
        listOfflineUsers->setMaximumSize(QSize(258, 16777215));

        sideLayout->addWidget(listOfflineUsers);


        horizontalLayout->addLayout(sideLayout);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(mainToolBar->sizePolicy().hasHeightForWidth());
        mainToolBar->setSizePolicy(sizePolicy3);
        mainToolBar->setIconSize(QSize(40, 40));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setSizeGripEnabled(false);
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1034, 21));
        menuMain = new QMenu(menuBar);
        menuMain->setObjectName(QString::fromUtf8("menuMain"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        MainWindow->setMenuBar(menuBar);

        mainToolBar->addSeparator();
        mainToolBar->addAction(actionCopy);
        mainToolBar->addAction(actionCut);
        mainToolBar->addAction(actionPaste);
        mainToolBar->addSeparator();
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionBold);
        mainToolBar->addAction(actionItalic);
        mainToolBar->addAction(actionUnderlined);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionAlignLeft);
        mainToolBar->addAction(actionAlignCenter);
        mainToolBar->addAction(actionAlignRight);
        mainToolBar->addAction(actionAlignJustify);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionTestColor);
        mainToolBar->addAction(actionTestCursor);
        mainToolBar->addAction(actionTestDisconnect);
        menuBar->addAction(menuMain->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuMain->addAction(actionConnect);
        menuMain->addAction(actionExport_to_PDF);
        menuMain->addAction(actionInvite);
        menuMain->addSeparator();
        menuMain->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "SimulPad", nullptr));
        actionCopy->setText(QCoreApplication::translate("MainWindow", "Copy", nullptr));
        actionCut->setText(QCoreApplication::translate("MainWindow", "Cut", nullptr));
        actionPaste->setText(QCoreApplication::translate("MainWindow", "Paste", nullptr));
        actionBold->setText(QCoreApplication::translate("MainWindow", "Bold", nullptr));
        actionItalic->setText(QCoreApplication::translate("MainWindow", "Italic", nullptr));
        actionUnderlined->setText(QCoreApplication::translate("MainWindow", "Underlined", nullptr));
        actionAlignLeft->setText(QCoreApplication::translate("MainWindow", "AlignLeft", nullptr));
        actionAlignCenter->setText(QCoreApplication::translate("MainWindow", "AlignCenter", nullptr));
        actionAlignRight->setText(QCoreApplication::translate("MainWindow", "AlignRight", nullptr));
        actionConnect->setText(QCoreApplication::translate("MainWindow", "Connect...", nullptr));
        actionExport_to_PDF->setText(QCoreApplication::translate("MainWindow", "Export to PDF", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionAlignJustify->setText(QCoreApplication::translate("MainWindow", "AlignJustify", nullptr));
        actionInvite->setText(QCoreApplication::translate("MainWindow", "Invite to Collaborate", nullptr));
        actionTestCursor->setText(QCoreApplication::translate("MainWindow", "TestCursor", nullptr));
        actionTestDisconnect->setText(QCoreApplication::translate("MainWindow", "TestDisconnect", nullptr));
#if QT_CONFIG(tooltip)
        actionTestDisconnect->setToolTip(QCoreApplication::translate("MainWindow", "TestDisconnect", nullptr));
#endif // QT_CONFIG(tooltip)
        actionTestColor->setText(QCoreApplication::translate("MainWindow", "TestColor", nullptr));
#if QT_CONFIG(tooltip)
        actionTestColor->setToolTip(QCoreApplication::translate("MainWindow", "Highlights the text based on who entered it", nullptr));
#endif // QT_CONFIG(tooltip)
        textEditShared->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                                          "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                                          "p, li { white-space: pre-wrap; }\n"
                                                                          "</style></head><body style=\" font-family:'Calibri'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
                                                                          "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        myUsername->setText(QCoreApplication::translate("MainWindow", "dario.patti90", nullptr));
        myStatus->setText(QCoreApplication::translate("MainWindow", "Online", nullptr));
        myAvatar->setText(QString());
        myLed->setText(QString());
        onlineRollButton->setText(QCoreApplication::translate("MainWindow", "Online Users", nullptr));
        offlineRollButton->setText(QCoreApplication::translate("MainWindow", "Offline Users", nullptr));
        mainToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "SynkEditor", nullptr));
        menuMain->setTitle(QCoreApplication::translate("MainWindow", "Main", nullptr));
        menuAbout->setTitle(QCoreApplication::translate("MainWindow", "About", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
