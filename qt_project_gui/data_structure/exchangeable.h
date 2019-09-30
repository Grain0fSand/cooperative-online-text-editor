#ifndef EXCHANGEABLE_H
#define EXCHANGEABLE_H

#include <iostream>

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

}

#endif // EXCHANGEABLE_H