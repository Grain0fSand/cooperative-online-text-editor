#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <SQLiteCpp/Database.h>
#include "sha256.h"
#include "exchangeable_data.h"

class Database {

public:
    Database();

    int userLogged(std::string token);
    std::string userLogin(std::string username,std::string password);
    int userRegistration(std::string email,std::string username,std::string password,std::string image);
    std::string newDocument(std::string uid,std::string docName);
    std::string getPartecipants(std::string token,std::string docName);
    int updateUserData(std::string token,std::string username,std::string image,std::string password);
    void addPartecipant(std::string docId,std::string uid);
    void updateTimestamp(std::string docId,std::string uid);
    void insertCrdt(std::string crdt_json,std::string uid,std::string docId);
    std::vector<exchangeable_data::send_data> getCrdtUser(std::string lastCrdtId,std::string uid,std::string docId,std::string remoteCursor);
    std::vector<exchangeable_data::send_data> getOnlineUsers(std::string lastCrdtId,std::string uid,std::string docId);

private:
    SQLite::Database db;
    static std::string dbUri;
    std::map<std::string,int> sessionLogged;

    std::string hashed_pass(std::string pass);
    std::string random_string(size_t length);

    void updateTimestamp(std::string docId, std::string uid, std::string remoteCursor);
};

#endif //DATABASE_H