//
// Created by Elia Migliore on 28/09/2019.
//

#ifndef TRANSLATOR_SESSION_DATA_H
#define TRANSLATOR_SESSION_DATA_H

#include "../forms/mainwindow.h"
#include "../forms/loginwindow.h"
#include "../forms/mytextedit.h"

/*
 * this class represent all the data
 * avaiable after a webrequest
 */
namespace Ui {
    class SessionData;
}

class MainWindow;
class LoginWindow;

class SessionData {


public:

    std::string docId;
    std::string docName;
    std::string userId;
    std::string token;
    std::string lastCrdtId;
    std::string username;
    std::string password;
    QPixmap avatar;
    std::vector<UserTag> usersList;
    std::vector<exchangeable_data::user> onlineUsers;
    std::map<int,QColor> userColorMap;

    bool go_down = true;
    bool isUserOnline = false;
    bool isLoginCorrect = false;
    bool skipChanges = false;
    bool youWannaLogin = false;

    MyTextEdit* myTextEditPointer;
    LoginWindow* loginWindowPointer;
    MainWindow* mainWindowPointer;

    static SessionData& accessToSessionData() {
        static SessionData instance;

        return instance;
    }

private:
    explicit SessionData() = default;
    ~SessionData() = default;
    // TODO: add list of user
    // TODO: add list of cursor
    // TODO: add list of online user
};

#endif //TRANSLATOR_SESSION_DATA_H
