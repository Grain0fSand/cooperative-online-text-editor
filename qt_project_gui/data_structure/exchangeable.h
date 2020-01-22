#ifndef EXCHANGEABLE_H
#define EXCHANGEABLE_H

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
        user() : id(""),username(""),image(""),lastCursorPosition(),email("") {}
        user(std::string id,std::string email,std::string username,
             std::string image,std::pair<int,int> lastCursorPosition): id(id),username(username),image(image),
                                                                    lastCursorPosition(lastCursorPosition),email(email)
        {}

        user& operator=(const user &source) {
            if(this != &source) {
                this->id = source.id;
                this->email = source.id;
                this->username = source.username;
                this->image = source.image;
                this->lastCursorPosition = source.lastCursorPosition;
            }
            return *this;
        }

        bool operator<(const user &other) const {
            return this->id < other.id;
        }

        bool operator==(const user &other) const {
            return this->id == other.id;
        }

        bool operator!=(const user &other) const {
            return this->id != other.id;
        }

        std::string id;
        std::string username;
        std::string image;
        std::pair<int,int> lastCursorPosition;
        std::string email;

        json get_json() {
            return json{{"id", this->id},{"username", this->username},{"image", this->image}, {"curson_json", this->lastCursorPosition},{"email", this->email}};
        }

        static void from_json(user& d,const json& j){
            d.id = j.at("id").get<std::string>();
            d.username = j.at("username").get<std::string>();
            d.image = j.at("image").get<std::string>();

            std::string str = j.at("cursor_json").get<std::string>();
            int index = str.find(',');
            d.lastCursorPosition = std::make_pair(std::stoi(str.substr(0,index)), std::stoi(str.substr(index+1)));

            d.email = j.at("email").get<std::string>();
        }
    };

}

#endif // EXCHANGEABLE_H