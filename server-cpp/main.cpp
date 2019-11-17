#include <nlohmann/json.hpp>
#include "database.h"
#include "crow_all.h"

std::string Database::dbUri = "../pds.db";


int main() {
    Database db;
    crow::SimpleApp app;

    CROW_ROUTE(app,"/try_registration")
            .methods("GET"_method)
                ([&](const crow::request& req){
                    auto params = req.url_params;

                    if(params.get("email") == nullptr || params.get("username") == nullptr
                       || params.get("password") == nullptr || params.get("image")==nullptr)
                        return crow::response(500);

                    std::string email = params.get("email");
                    std::string username = params.get("username");
                    std::string password = params.get("password");
                    std::string image = params.get("image");

                    int replyCode = db.userRegistration(email,username,password,image);

                    return crow::response{std::to_string(replyCode)};
                });

    CROW_ROUTE(app,"/new_doc")
            .methods("GET"_method)
                    ([&](const crow::request& req){
                        auto params = req.url_params;

                        if(params.get("token") == nullptr || params.get("docName") == nullptr)
                            return crow::response(500);

                        std::string token = params.get("token");
                        std::string docName = params.get("docName");

                        std::string replyString = db.newDocument(token,docName);

                        return crow::response{replyString};
                    });

    CROW_ROUTE(app,"/get_partecipants")
            .methods("GET"_method)
                    ([&](const crow::request& req){
                        auto params = req.url_params;

                        if(params.get("token") == nullptr || params.get("docName") == nullptr)
                            return crow::response(500);

                        std::string token = params.get("token");
                        std::string docName = params.get("docName");

                        std::string dbReply = db.getPartecipants(token,docName);

                        return crow::response{dbReply};
                    });

    CROW_ROUTE(app,"/try_login")
            .methods("GET"_method)
                    ([&](const crow::request& req){
                        auto params = req.url_params;

                        if(params.get("username") == nullptr || params.get("password") == nullptr)
                            return crow::response(500);

                        std::string username = params.get("username");
                        std::string password = params.get("password");

                        std::string dbReply = db.userLogin(username,password);

                        return crow::response{dbReply};
                    });

    CROW_ROUTE(app,"/update_user_data")
           .methods("GET"_method)
                ([&](const crow::request& req){

                    auto params = req.url_params;

                    if(params.get("token") == nullptr || params.get("username") == nullptr
                       || params.get("avatar") == nullptr || params.get("password") == nullptr)
                        return crow::response(500);

                    std::string token = params.get("token");
                    std::string username = params.get("username");
                    std::string avatar = params.get("avatar");
                    std::string password = params.get("password");

                    int dbReply = db.updateUserData(token,username,avatar,password);

                    return crow::response{std::to_string(dbReply)};

                });

    // to test launch and open
    // http://localhost:6969/get_crdt?token=1&lastcrdt=&docId=1&remoteCursor=0
    CROW_ROUTE(app,"/get_crdt")
            .methods("GET"_method)
                    ([&](const crow::request& req){
                        auto params = req.url_params;

                        if(params.get("token") == nullptr || params.get("lastcrdt") == nullptr
                            || params.get("docId") == nullptr || params.get("remoteCursor") == nullptr)
                            // TODO: see remoteCursor param
                            return crow::response(500);

                        std::string token = params.get("token");
                        std::string lastcrdt = params.get("lastcrdt");
                        std::string docId = params.get("docId");
                        std::string remoteCursor = params.get("remoteCursor");

                        int idUser = db.userLogged(token);

                        if(idUser<0)
                            return crow::response(403);

                        std::vector<exchangeable_data::send_data> d = db.getCrdtUser(lastcrdt,std::to_string(idUser),docId,remoteCursor);
                        // TODO: check if now gui crash
                        json j;

                        for(exchangeable_data::send_data t : d){
                            j.push_back(t.get_json());
                        }

                        std::ostringstream os;

                        // fill the reply
                        os << j;

                        return crow::response{os.str()};
                    });

    // to test launch and open
    // TODO: add remoteCursor parameter
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

    app.port(6969)
    //.multithreaded() // active only if you need more performance
    .run();

    return 0;
}




/*
    QUERY INSERIMENTO DB:
    INSERT INTO user VALUES(NULL,'user','5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8','dario.patti@elia.migliore.com','image');
    INSERT INTO user_document_request VALUES(1,1,datetime('now'),'aaa')
    INSERT INTO document VALUES(NULL,'documento di prova')
    INSERT INTO crdt(idDoc,idUser,crdt_json)  VALUES(NULL,1,'crdt_json')
    insert into crdt_delvery(idCrdt,idUser,idDocument) values(1,1,1)
 */