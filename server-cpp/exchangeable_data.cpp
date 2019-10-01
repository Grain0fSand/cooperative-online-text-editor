#include "exchangeable_data.h"

void exchangeable_data::to_json(json& j, const exchangeable_data::send_data& d) {
    j = json{{"id", d.id}, {"crdt", d.crdt}};
}

void exchangeable_data::from_json(const json& j, exchangeable_data::send_data& d) {
    d.id = j.at("id").get<std::string>();
    d.crdt = j.at("crdt").get<std::string>();
}