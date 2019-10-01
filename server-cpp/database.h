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
    void userLogin(std::string username,std::string password);
    void userRegistration(std::string email,std::string username,std::string password);
    void addPartecipant(std::string docId,std::string uid);
    void updateTimestamp(std::string docId,std::string uid);
    void insertCrdt(std::string crdt_json,std::string uid,std::string docId);
    std::vector<exchangeable_data::send_data> getCrdtUser(std::string lastCrdtId,std::string uid,std::string docId);

private:
    SQLite::Database db;
    static std::string dbUri;
    std::map<std::string,int> sessionLogged;

    std::string hashed_pass(std::string pass);
    std::string random_string(size_t length);
};

#endif //DATABASE_H