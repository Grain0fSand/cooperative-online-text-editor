#include "forms/loginwindow.h"

#include "forms/mainwindow.h"
#include "utility/image_utility.h"
#include "data_structure/crdt.h"
#include "data_structure/action.h"

#include <QApplication>
#include <QStyleFactory>
#include <nlohmann/json.hpp>
#include "utility/json_comunicator.h"
#include "data_structure/exchangeable.h"

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
