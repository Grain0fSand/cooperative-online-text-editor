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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

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
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionConnect;
    QAction *actionAccount_Settings;
    QAction *actionExport_to_PDF;
    QAction *actionExit;
    QAction *actionAlignJustify;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QWidget *docFrame;
    QGridLayout *docFrameLayout;
    QTextEdit *textEditShared;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuMain;
    QMenu *menuAbout;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1004, 1082);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/logoIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/ic_content_copy_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon1);
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/ic_content_cut_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon2);
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/ic_content_paste_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon3);
        actionBold = new QAction(MainWindow);
        actionBold->setObjectName(QString::fromUtf8("actionBold"));
        actionBold->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/ic_format_bold_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBold->setIcon(icon4);
        actionItalic = new QAction(MainWindow);
        actionItalic->setObjectName(QString::fromUtf8("actionItalic"));
        actionItalic->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/ic_format_italic_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionItalic->setIcon(icon5);
        actionUnderlined = new QAction(MainWindow);
        actionUnderlined->setObjectName(QString::fromUtf8("actionUnderlined"));
        actionUnderlined->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/ic_format_underlined_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUnderlined->setIcon(icon6);
        actionAlignLeft = new QAction(MainWindow);
        actionAlignLeft->setObjectName(QString::fromUtf8("actionAlignLeft"));
        actionAlignLeft->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/ic_format_align_left_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAlignLeft->setIcon(icon7);
        actionAlignCenter = new QAction(MainWindow);
        actionAlignCenter->setObjectName(QString::fromUtf8("actionAlignCenter"));
        actionAlignCenter->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/ic_format_align_center_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAlignCenter->setIcon(icon8);
        actionAlignRight = new QAction(MainWindow);
        actionAlignRight->setObjectName(QString::fromUtf8("actionAlignRight"));
        actionAlignRight->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/ic_format_align_right_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAlignRight->setIcon(icon9);
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionUndo->setEnabled(false);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/ic_undo_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon10);
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionRedo->setEnabled(false);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/ic_redo_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon11);
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionAccount_Settings = new QAction(MainWindow);
        actionAccount_Settings->setObjectName(QString::fromUtf8("actionAccount_Settings"));
        actionExport_to_PDF = new QAction(MainWindow);
        actionExport_to_PDF->setObjectName(QString::fromUtf8("actionExport_to_PDF"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAlignJustify = new QAction(MainWindow);
        actionAlignJustify->setObjectName(QString::fromUtf8("actionAlignJustify"));
        actionAlignJustify->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/PdsProject.app/Contents/Resources/img/ic_format_align_justify_36pt_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAlignJustify->setIcon(icon12);
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
        textEditShared = new QTextEdit(docFrame);
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

        docFrameLayout->addWidget(textEditShared, 1, 1, 1, 1);


        horizontalLayout->addWidget(docFrame, 0, Qt::AlignHCenter);

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
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1004, 21));
        menuMain = new QMenu(menuBar);
        menuMain->setObjectName(QString::fromUtf8("menuMain"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        MainWindow->setMenuBar(menuBar);

        mainToolBar->addAction(actionUndo);
        mainToolBar->addAction(actionRedo);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionCopy);
        mainToolBar->addAction(actionCut);
        mainToolBar->addAction(actionPaste);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionBold);
        mainToolBar->addAction(actionItalic);
        mainToolBar->addAction(actionUnderlined);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionAlignLeft);
        mainToolBar->addAction(actionAlignCenter);
        mainToolBar->addAction(actionAlignRight);
        mainToolBar->addAction(actionAlignJustify);
        menuBar->addAction(menuMain->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuMain->addAction(actionConnect);
        menuMain->addAction(actionAccount_Settings);
        menuMain->addAction(actionExport_to_PDF);
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
        actionUndo->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
        actionRedo->setText(QCoreApplication::translate("MainWindow", "Redo", nullptr));
        actionConnect->setText(QCoreApplication::translate("MainWindow", "Connect...", nullptr));
        actionAccount_Settings->setText(QCoreApplication::translate("MainWindow", "Account Settings", nullptr));
        actionExport_to_PDF->setText(QCoreApplication::translate("MainWindow", "Export to PDF", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionAlignJustify->setText(QCoreApplication::translate("MainWindow", "AlignJustify", nullptr));
        textEditShared->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Calibri'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Bangla MN'; font-size:16pt;\"><br /></p></body></html>", nullptr));
        textEditShared->setPlaceholderText(QCoreApplication::translate("MainWindow", "inizia ad editare qui!", nullptr));
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
