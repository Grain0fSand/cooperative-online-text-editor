#include <QApplication>
#include <QStyleFactory>
#include "forms/mainwindow.h"
#include "forms/loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion")); // for uniform style
    LoginWindow::getInstance().exec();
    if(LoginWindow::getInstance().isLoginCorrect()) {
        MainWindow::getInstance().showMaximized();

        return a.exec();
    }
}
