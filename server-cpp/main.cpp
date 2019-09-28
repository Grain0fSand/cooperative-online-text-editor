#include <iostream>
#include <SQLiteCpp/Database.h>
#include <vector>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "crow_all.h"
#include "sha256.h"
#include "exchangable_data.h"

class Database {
private:
    SQLite::Database db;
    static std::string dbUri;
    std::map<std::string,int> sessionLogged;

    /*
     * function private for hash
     */
    std::string hashed_pass(std::string pass){
        return sha256(pass + "my crazy random salt porcamadonna PDS");
    }

    std::string random_string( size_t length )
    {
        auto randchar = []() -> char
        {
            const char charset[] =
                    "0123456789"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
        };
        std::string str(length,0);
        std::generate_n( str.begin(), length, randchar );
        return str;
    }

public:
    Database() : db(dbUri,SQLite::OPEN_READWRITE) {
        // TODO: only for test, remove it
        std::cout << "db opened";
        sessionLogged["1"] = 1;
        sessionLogged["2"] = 1;
    }

    int userLogged(std::string token){
        if (sessionLogged.find(token) != sessionLogged.end())
            return sessionLogged[token];

        return -1;
    }

    void login(std::string username,std::string password){
        std::string hashedPass = hashed_pass(password);
        bool isLoginCorrect = true;


        // TODO: check login is correct
        if (isLoginCorrect) {
            std::string token = random_string(40);
            int id = 15;

            sessionLogged[token] = id;
        }
    }

    //TODO: ricordati di inviare tutto lo storico dei crdt
    void addPartecipant(std::string docId,std::string uid){
        std::string sql = "INSERT INTO user_document_request VALUES(" + uid + "," + docId + ",datetime('now'))";
        SQLite::Statement query(db,sql);
        query.exec();
    }

    void updateTimestamp(std::string docId,std::string uid){
        std::string sql = "UPDATE user_document_request SET lastReq = datetime('now') WHERE idUser = " + uid + " AND idDocument = " + docId + ";)";
        SQLite::Statement query(db,sql);
        query.exec();
    }

    /*
     * the operation of insert crdt create the crdt, and after create a notification
     * for each user subscribed to the document, the subscription is seen if a user
     * has a row inside the table user_document_request
     */
    void insertCrdt(std::string crdt_json,std::string uid,std::string docId){
        std::string sql = "INSERT INTO crdt(idDoc,idUser,crdt_json)  VALUES(" + docId + "," + uid + ",'" + crdt_json + "');";

        SQLite::Statement queryUpdate(db,sql);
        auto res = queryUpdate.exec();
        int idCrdtInserted = db.getLastInsertRowid();

        sql = "SELECT idUser FROM user_document_request WHERE idDocument=" + docId + ";";
        SQLite::Statement idGetter(db,sql);
        std::vector<int> ids;


        while (idGetter.executeStep()){
            ids.push_back(idGetter.getColumn(0));
        }

        std::stringstream ss{uid};

        int creator_id;

        ss >> creator_id;

        ids.erase(std::remove(ids.begin(),ids.end(),creator_id),ids.end());
        std::string crdtId = std::to_string(idCrdtInserted);

        for(int usrId : ids){
            sql = "insert into crdt_delvery(idDoc,idCrdt,idUser) values(" + docId + "," + crdtId + "," + std::to_string(usrId) + ")";
            SQLite::Statement insertNotification(db,sql);
            insertNotification.exec();
        }

    }

    /*
     * it returns the list of unseen crdt given a user, a document and the last
     * seen document; it ensure also the statistics of witch crdt has been seen
     * by a user
     */
    std::vector<exchangable_data::send_data> getCrdtUser(std::string lastCrdtId,std::string uid,std::string docId){
        std::string sql;

        if (lastCrdtId.compare("")!=0){
            sql = "UPDATE crdt_delvery SET delivered = '1' WHERE idDoc=" + docId + " AND idCrdt <=" + lastCrdtId + " AND idUser = " + uid + ";";

            SQLite::Statement queryUpdate(db,sql);
            queryUpdate.exec();


            sql = "SELECT id,crdt_json FROM crdt WHERE id IN (SELECT idCrdt from crdt_delvery WHERE idUser=" + uid + " AND idDoc=" + docId + "  AND delivered=0)";
        } else {
            sql = "SELECT id,crdt_json FROM crdt WHERE id IN (SELECT idCrdt from crdt_delvery WHERE idUser=" + uid + " AND idDoc=" + docId + ")";
        }

        std::cout << sql;

        SQLite::Statement query(db, sql);
        std::vector<exchangable_data::send_data> vect;

        while (query.executeStep()){
            std::string id = query.getColumn(0);
            std::string crdt_json = query.getColumn(1);

            vect.push_back(exchangable_data::send_data(id,crdt_json));
        }

        return vect;
    }


};

std::string Database::dbUri = "../pds.db";


int main() {
    Database db;
    crow::SimpleApp app;

    CROW_ROUTE(app,"/register_user")
    .methods("GET"_method)
    ([&](const crow::request& req){
        auto params = req.url_params;

        // if error
        //return crow::response(400);

        std::string par = params.get("username");
        std::ostringstream os;

        // fill the reply
        os << par;
        os << "che schifo i voti di cabodi madonna immacoolata";

        return crow::response{os.str()};
    });

    CROW_ROUTE(app,"/update_user")
    .methods("GET"_method)
            ([&](const crow::request& req){
                return "bello stai calmo che non ho ancora finito! torna più tardi";
            });


    CROW_ROUTE(app,"/get_image_user")
    .methods("GET"_method)
            ([&](const crow::request& req){
                return "senti mettici dentro la foto di un cazzo  tanto a cabodi piace perfino di più";
            });

    // to test launch and open
    // http://localhost:8080/get_crdt?token=1&lastcrdt=&docId=1
    CROW_ROUTE(app,"/get_crdt")
            .methods("GET"_method)
                    ([&](const crow::request& req){
                        auto params = req.url_params;

                        if(params.get("token") == nullptr || params.get("lastcrdt") == nullptr
                        || params.get("docId") == nullptr)
                            return crow::response(500);


                        // TODO: replace the simple id to the hashmap of token
                        std::string uid = params.get("token");
                        std::string lastcrdt = params.get("lastcrdt");
                        std::string docId = params.get("docId");

                        int idUser = db.userLogged(uid);
                        if(idUser<0)
                            return crow::response(403);

                        std::vector<exchangable_data::send_data> d = db.getCrdtUser(lastcrdt,std::to_string(idUser),docId);
                        json j = d;
                        std::ostringstream os;

                        // fill the reply
                        os << j;

                        return crow::response{os.str()};
                    });

    // to test launch and open
    // http://localhost:8080/push_crdt?token=1&crdt=mannaggia il porco&docId=1
    CROW_ROUTE(app,"/push_crdt")
            .methods("GET"_method)
                    ([&](const crow::request& req){
                        auto params = req.url_params;

                        if(params.get("token") == nullptr || params.get("crdt") == nullptr
                           || params.get("docId") == nullptr)
                            return crow::response(500);


                        // TODO: replace the simple id to the hashmap of token
                        std::string uid = params.get("token");
                        std::string crdt = params.get("crdt");
                        std::string docId = params.get("docId");

                        int idUser = db.userLogged(uid);
                        if(idUser<0)
                            return crow::response(403);

                        db.insertCrdt(crdt,std::to_string(idUser),docId);

                        std::ostringstream os;

                        // fill the reply
                        os << "1";

                        return crow::response{os.str()};
                    });

    app.port(8080)
    //.multithreaded() // active only if you need more performance
    .run();

    return 0;
}




/*
    QUERY INSERIMENTO DB:
    INSERT INTO user VALUES(NULL,'user','5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8','dario.patti@elia.migliore.com','image');
    INSERT INTO user_document_request VALUES(1,1,datetime('now'))
    INSERT INTO document VALUES(NULL,'documento di prova')
    INSERT INTO crdt(idDoc,idUser,crdt_json)  VALUES(NULL,1,'crdt_json')
    insert into crdt_delvery(idCrdt,idUser,idDocument) values(1,1,1)
 */