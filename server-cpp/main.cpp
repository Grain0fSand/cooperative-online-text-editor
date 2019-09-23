#include <iostream>
//#include "crow_all_mac.h"

#include <mongoc/mongoc.h>


#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::types::b_date;
using std::chrono::system_clock;
using bsoncxx::builder::basic::make_document;

class Database {
private:
    static const std::string connUri;
    mongocxx::instance inst;
    mongocxx::client connection;

public:
    Database() : inst{},connection{mongocxx::uri{connUri}}{
    }

    std::unique_ptr<std::string> insertUser(std::string name,std::string password,std::string image,std::string username){
        auto collection = connection["pds-db"]["users"];
        bsoncxx::builder::stream::document docBuff;

        docBuff << "name" << name;
        docBuff << "password" << password;
        docBuff << "image" << image;
        docBuff << "username" << username;

        auto res = collection.insert_one(docBuff.view());

        return std::make_unique<std::string>(res.get().inserted_id().get_oid().value.to_string());
    }

    void getUser(){
        auto collection = connection["pds-db"]["users"];
        auto cursor = collection.find({});

        for (auto&& doc : cursor) {
            std::cout << bsoncxx::to_json(doc) << std::endl;
        }
    }

    std::unique_ptr<std::string> createDocument(std::string docName,std::string uid){
        std::string docId{"DOC_" + docName + uid};
        auto collection = connection["pds-db"][docId];
        bsoncxx::builder::stream::document docBuff;

        docBuff << "name" << docName;
        docBuff << "partecipant" << make_array(uid);
        docBuff << "crdt" << make_array();
        docBuff << "messages" << make_array();
        docBuff << "online users" << make_document(kvp(uid,b_date(system_clock::now())));


        auto res = collection.insert_one(docBuff.view());

        return std::make_unique<std::string>(res.get().inserted_id().get_oid().value.to_string());
    }


    void addCrdtToDocument(std::string docId,std::string uid,std::string crdt,std::string docName){
        auto collection = connection["pds-db"][docId];

        auto res = collection.update_one(make_document(kvp("name", docName)),
                make_document(kvp("$push",make_document(kvp("messages",make_document(kvp("cia","mondo")))))));

        //std::cout << res->result().upserted_ids();

    }


    void addPartecipantToDocument(std::string docName,std::string uid,std::string docId){
        auto collection = connection["pds-db"][docId];

        collection.update_one(make_document(kvp("name", docName)),make_document(kvp("$push",make_document(kvp("partecipant",uid)))));
        std::cout << "done!";
    }

    void viewPartecipantToDocument(std::string docId){
        auto collection = connection["pds-db"][docId];
        auto obj = collection.find_one({}).value().view();
        auto partecipant = obj["partecipant"].get_array();
        std::vector<std::string> partecipants{};

        for(bsoncxx::v_noabi::array::element p : partecipant.value){
            std::string val = p.get_utf8().value.to_string();
            partecipants.push_back(val);
        }
    }

    bool checkPartecipantToDocument(std::string docId,std::string uid){
        auto collection = connection["pds-db"][docId];
        auto obj = collection.find_one({}).value().view();

        bsoncxx::stdx::string_view res = obj["name"].get_utf8();
        bsoncxx::stdx::string_view exampleArray = obj["crdt"].get_array().value.begin()->get_utf8(); // .get_document().value["prova"]
        std::chrono::microseconds exampleDate = obj["online users"]["prova"].get_date().value;

        auto exist = obj["online users"]["aaa"];

        if (exist){
            return true;
        } else {
            return false;
        }

        // {"online users.prova": { $exists : true } }

        // std::cout << res;
        // std::cout << exampleDate.count();
        // std::cout << exampleArray;
    }

};

const std::string Database::connUri{"mongodb://PDS-admin:123stella@47.53.242.167:27017"};

int main() {

    Database db;
    db.addCrdtToDocument("DOC_provaprova","giggino","crdt di prova","prova");
    /*
     *
     * working code for generating a webpage with plain hello work response
     *
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]{
        return "Hello, world!";
    });

    CROW_ROUTE(app, "/amazon")
            ([]{
                return "Hello, amazon world!";
            });

    app
    .port(8080)
    .multithreaded()
    .run();
     */
}

void tests(){
    // creation of a user
    Database db;
    auto ptr = db.insertUser("elia","ciao","ajisdasjida","eliax1996");
    std::cout<<ptr;
}