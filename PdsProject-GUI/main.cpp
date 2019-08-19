#include "mainwindow.h"
#include "image_utility.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setStyle(QStyleFactory::create("Fusion")); // for uniform style
    MainWindow w;
    w.show();

    QColor c;
    c.red();
    c.setRgb(10,12,15);

    create_svg(c,120);


    return a.exec();
}
