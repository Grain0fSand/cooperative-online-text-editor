#include "loginwindow.h"
#include "mainwindow.h"
#include "image_utility.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //LoginWindow l;
    //l.exec();
    //if(l.isLoginCorrect()) {
        MainWindow w;
        w.show();

        return a.exec();
    //}
    //a.setStyle(QStyleFactory::create("Fusion")); // for uniform style

    /*QColor c;
    c.red();
    c.setRgb(10,12,15);

    create_svg(c,120);

*/
}
