#include <QApplication>
#include <QStyleFactory>
#include "forms/mainwindow.h"
#include "forms/loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //LoginWindow::getInstance().exec();
    //if(LoginWindow::getInstance().isLoginCorrect()) {
        MainWindow::getInstance().show();

        a.setStyle(QStyleFactory::create("Fusion")); // for uniform style

        return a.exec();
    //}
}
