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

        send_data(std::string id,std::string crdt) : crdt(crdt),id(id) {}
    };


    void to_json(json& j, const send_data& d);

    void from_json(const json& j, send_data& d);

}

#endif //EXCHANGEABLE_DATA_H