#include "forms/loginwindow.h"
#include "forms/mainwindow.h"
#include "utility/image_utility.h"
#include "data_structure/crdt.h"
#include "data_structure/action.h"
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
