#-------------------------------------------------
#
# Project created by QtCreator 2019-08-04T18:52:45
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PdsProject
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        action.cpp \
        json_comunicator.cpp \
        loginwindow.cpp \
        main.cpp \
        mainwindow.cpp \
        mytextedit.cpp \
        remotecursor.cpp \
        smtpclient.cpp \
        usertag.cpp

HEADERS += \
        action.h \
        json_comunicator.h \
        loginwindow.h \
        mainwindow.h \
        mytextedit.h \
        online_synchronizer.h \
        remotecursor.h \
        shared_editor.h \
        smtpclient.h \
        usertag.h

FORMS += \
        loginwindow.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
