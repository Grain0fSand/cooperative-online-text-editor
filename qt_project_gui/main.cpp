#include "forms/loginwindow.h"

#include "forms/mainwindow.h"
#include "utility/image_utility.h"
#include "data_structure/crdt.h"
#include "data_structure/action.h"

#include <QApplication>
#include <QStyleFactory>
#include <nlohmann/json.hpp>
#include "utility/json_comunicator.h"
#include "exchangeable.h"

int main(int argc, char *argv[])
{
//    exchangable_data::send_data b("ff","ff");
//    json j = b;
//    std::cout << j.dump();
    QApplication a(argc, argv);
//    LoginWindow::getInstance().exec();
//    if(LoginWindow::getInstance().isLoginCorrect()) {
        MainWindow::getInstance().show();

        a.setStyle(QStyleFactory::create("Fusion")); // for uniform style

        return a.exec();
//    }
    //a.setStyle(QStyleFactory::create("Fusion")); // for uniform style

    /*QColor c;
    c.red();
    c.setRgb(10,12,15);

    create_svg(c,120);

*/
//    ActionWrapper a(Action(), std::make_pair((int)1,(int)1), std::vector<std::pair<int,int>>());
//    json_serializer::sendAction(a);
}
