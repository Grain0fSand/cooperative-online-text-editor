#include <iostream>
#include "crow_all_mac.h"

#include <mongoc/mongoc.h>


#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>



int main() {

    mongocxx::instance inst{};

    static const std::string uri = std::string("mongodb://PDS-admin:123stella@47.53.242.167:27017");

    mongocxx::client conn{mongocxx::uri{uri}};

    bsoncxx::builder::stream::document document{};

    auto collection = conn["pds-db"]["testcollection"];
    document << "hello" << "world";

    collection.insert_one(document.view());
    auto cursor = collection.find({});

    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }

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