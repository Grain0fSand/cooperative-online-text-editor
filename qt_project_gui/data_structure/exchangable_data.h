//
// Created by Elia Migliore on 28/09/2019.
//

#ifndef SQLITE_EXCHANGABLE_DATA_H
#define SQLITE_EXCHANGABLE_DATA_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

namespace exchangable_data {

    class send_data {
    public:
        std::string id;
        std::string crdt;

        send_data(std::string id,std::string crdt) : crdt(crdt),id(id) {}
    };


    void to_json(json& j, const send_data& d) {
        j = json{{"id", d.id}, {"crdt", d.crdt}};
    }

    void from_json(const json& j, send_data& d) {
        d.id = j.at("id").get<std::string>();
        d.crdt = j.at("crdt").get<std::string>();
    }

}

#endif //SQLITE_EXCHANGABLE_DATA_H
