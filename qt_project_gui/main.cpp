#include <QApplication>
#include <QStyleFactory>
#include "forms/mainwindow.h"
#include "forms/loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SessionData::accessToSessionData();
    a.setStyle(QStyleFactory::create("Fusion")); // for uniform style
    int exitCode=0;

    LoginWindow loginWindow;
    do {
        loginWindow.exec();

        if(SessionData::accessToSessionData().isLoginCorrect) {
            MainWindow mainWindow;
            mainWindow.showMaximized();

            exitCode = a.exec();
        } else break;
    } while(SessionData::accessToSessionData().youWannaLogin);

    return exitCode;
}
