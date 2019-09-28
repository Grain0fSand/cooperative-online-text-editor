
#ifndef TRANSLATOR_EXCHANGEABLE_H
#define TRANSLATOR_EXCHANGEABLE_H

using json = nlohmann::json;

//namespace json_serializer {
//    std::string sendAction(ActionWrapper &actionWrapper);
//    void receiveAction(json j);
//}

namespace exchangable_data {
    class send_data {
    public:
        send_data(){}
        std::string id;
        std::string crdt;

        send_data(std::string id,std::string crdt) :id(id),  crdt(crdt) {}
    };


    //void to_json(json& j, const send_data& d) {
        //j = json{{"id", d.id}, {"crdt", d.crdt}};
        //}

    //void from_json(const json& j, send_data& d) {
    //    d.id = j.at("id").get<std::string>();
    //    d.crdt = j.at("crdt").get<std::string>();
    //}
}

#endif //TRANSLATOR_EXCHANGEABLE_H
