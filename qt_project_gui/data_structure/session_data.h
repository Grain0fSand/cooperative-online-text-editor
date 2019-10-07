//
// Created by Elia Migliore on 28/09/2019.
//

#ifndef TRANSLATOR_SESSION_DATA_H
#define TRANSLATOR_SESSION_DATA_H

/*
 * this class represent all the data
 * avaiable after a webrequest
 */
class SessionData {
public:
    std::string docId;
    std::string docName;
    std::string token;
    std::string lastCrdtId;
    std::string username;
    std::string password;
    QPixmap avatar;
    bool status;
    // TODO: add list of user
    // TODO: add list of cursor
    // TODO: add list of online user
};

#endif //TRANSLATOR_SESSION_DATA_H
