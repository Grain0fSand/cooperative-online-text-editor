#ifndef EXCHANGEABLE_DATA_H
#define EXCHANGEABLE_DATA_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

namespace exchangeable_data {

    class send_data {
    public:
        send_data(){}
        std::string id;
        std::string crdt;

        send_data(std::string id,std::string crdt) :id(id),  crdt(crdt) {}

        json get_json() {
            return json{{"id", this->id}, {"crdt", this->crdt}};
        }

        static void from_json(send_data& d,const json& j){
            d.id = j.at("id").get<std::string>();
            d.crdt = j.at("crdt").get<std::string>();
        }
    };

    class user {
    public:
        user(){}
        user(std::string id,std::string username,
                std::string image,std::string lastCursorPositionJson): id(id),
                username(username),image(image),lastCursorPositionJson(lastCursorPositionJson)
                {}

        std::string id;
        std::string username;
        std::string image;
        std::string lastCursorPositionJson;
    };

}

#endif //EXCHANGEABLE_DATA_H