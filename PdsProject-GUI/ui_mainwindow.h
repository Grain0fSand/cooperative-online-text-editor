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
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonConnect;
    QPushButton *pushButtonPDF;
    QPushButton *pushButtonCopy;
    QPushButton *pushButtonPaste;
    QPushButton *pushButtonSettings;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButtonFont;
    QPushButton *pushButtonBold;
    QPushButton *pushButtonUnderlined;
    QPushButton *pushButtonItalic;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButtonAlignLeft;
    QPushButton *pushButtonAlignCenter;
    QPushButton *pushButtonAlignRight;
    QTextEdit *textEditShared;
    QListView *listViewUsers;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(900, 640);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMinimumSize(QSize(900, 600));
        horizontalLayout_3 = new QHBoxLayout(centralWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonConnect = new QPushButton(centralWidget);
        pushButtonConnect->setObjectName(QString::fromUtf8("pushButtonConnect"));
        sizePolicy.setHeightForWidth(pushButtonConnect->sizePolicy().hasHeightForWidth());
        pushButtonConnect->setSizePolicy(sizePolicy);
        pushButtonConnect->setIconSize(QSize(40, 40));
        pushButtonConnect->setCheckable(false);

        horizontalLayout->addWidget(pushButtonConnect);

        pushButtonPDF = new QPushButton(centralWidget);
        pushButtonPDF->setObjectName(QString::fromUtf8("pushButtonPDF"));
        sizePolicy.setHeightForWidth(pushButtonPDF->sizePolicy().hasHeightForWidth());
        pushButtonPDF->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButtonPDF);

        pushButtonCopy = new QPushButton(centralWidget);
        pushButtonCopy->setObjectName(QString::fromUtf8("pushButtonCopy"));
        sizePolicy.setHeightForWidth(pushButtonCopy->sizePolicy().hasHeightForWidth());
        pushButtonCopy->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButtonCopy);

        pushButtonPaste = new QPushButton(centralWidget);
        pushButtonPaste->setObjectName(QString::fromUtf8("pushButtonPaste"));
        sizePolicy.setHeightForWidth(pushButtonPaste->sizePolicy().hasHeightForWidth());
        pushButtonPaste->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButtonPaste);

        pushButtonSettings = new QPushButton(centralWidget);
        pushButtonSettings->setObjectName(QString::fromUtf8("pushButtonSettings"));
        sizePolicy.setHeightForWidth(pushButtonSettings->sizePolicy().hasHeightForWidth());
        pushButtonSettings->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButtonSettings);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButtonFont = new QPushButton(centralWidget);
        pushButtonFont->setObjectName(QString::fromUtf8("pushButtonFont"));

        verticalLayout_2->addWidget(pushButtonFont);

        pushButtonBold = new QPushButton(centralWidget);
        pushButtonBold->setObjectName(QString::fromUtf8("pushButtonBold"));

        verticalLayout_2->addWidget(pushButtonBold);

        pushButtonUnderlined = new QPushButton(centralWidget);
        pushButtonUnderlined->setObjectName(QString::fromUtf8("pushButtonUnderlined"));

        verticalLayout_2->addWidget(pushButtonUnderlined);

        pushButtonItalic = new QPushButton(centralWidget);
        pushButtonItalic->setObjectName(QString::fromUtf8("pushButtonItalic"));

        verticalLayout_2->addWidget(pushButtonItalic);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pushButtonAlignLeft = new QPushButton(centralWidget);
        pushButtonAlignLeft->setObjectName(QString::fromUtf8("pushButtonAlignLeft"));

        horizontalLayout_4->addWidget(pushButtonAlignLeft);

        pushButtonAlignCenter = new QPushButton(centralWidget);
        pushButtonAlignCenter->setObjectName(QString::fromUtf8("pushButtonAlignCenter"));

        horizontalLayout_4->addWidget(pushButtonAlignCenter);

        pushButtonAlignRight = new QPushButton(centralWidget);
        pushButtonAlignRight->setObjectName(QString::fromUtf8("pushButtonAlignRight"));

        horizontalLayout_4->addWidget(pushButtonAlignRight);


        verticalLayout_2->addLayout(horizontalLayout_4);


        horizontalLayout_2->addLayout(verticalLayout_2);

        textEditShared = new QTextEdit(centralWidget);
        textEditShared->setObjectName(QString::fromUtf8("textEditShared"));
        QFont font;
        font.setFamily(QString::fromUtf8("Helvetica"));
        font.setPointSize(16);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        textEditShared->setFont(font);

        horizontalLayout_2->addWidget(textEditShared);

        listViewUsers = new QListView(centralWidget);
        listViewUsers->setObjectName(QString::fromUtf8("listViewUsers"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listViewUsers->sizePolicy().hasHeightForWidth());
        listViewUsers->setSizePolicy(sizePolicy1);
        listViewUsers->setMinimumSize(QSize(433, 0));

        horizontalLayout_2->addWidget(listViewUsers);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButtonConnect->setText(QCoreApplication::translate("MainWindow", "connettiti", nullptr));
        pushButtonPDF->setText(QCoreApplication::translate("MainWindow", "esporta come pdf", nullptr));
        pushButtonCopy->setText(QCoreApplication::translate("MainWindow", "copia", nullptr));
        pushButtonPaste->setText(QCoreApplication::translate("MainWindow", "incolla", nullptr));
        pushButtonSettings->setText(QCoreApplication::translate("MainWindow", "impostazioni account", nullptr));
        pushButtonFont->setText(QCoreApplication::translate("MainWindow", "font e dimensioni", nullptr));
        pushButtonBold->setText(QCoreApplication::translate("MainWindow", "grassetto", nullptr));
        pushButtonUnderlined->setText(QCoreApplication::translate("MainWindow", "sottolineato", nullptr));
        pushButtonItalic->setText(QCoreApplication::translate("MainWindow", "corsivo", nullptr));
        pushButtonAlignLeft->setText(QCoreApplication::translate("MainWindow", "sinistra", nullptr));
        pushButtonAlignCenter->setText(QCoreApplication::translate("MainWindow", "centro", nullptr));
        pushButtonAlignRight->setText(QCoreApplication::translate("MainWindow", "destra", nullptr));
        textEditShared->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Helvetica'; font-size:16pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Bangla MN';\"><br /></p></body></html>", nullptr));
        textEditShared->setPlaceholderText(QCoreApplication::translate("MainWindow", "inizia ad editare qui!", nullptr));
        mainToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "SynkEditor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
