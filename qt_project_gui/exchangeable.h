
#ifndef TRANSLATOR_EXCHANGEABLE_H
#define TRANSLATOR_EXCHANGEABLE_H

using json = nlohmann::json;

namespace exchangable_data {
    class send_data {
    public:
        send_data(){}
        std::string id;
        std::string crdt;

        send_data(std::string id,std::string crdt) :id(id),  crdt(crdt) {}

        json get_json() {
            return json{{"id", this->id}, {"crdt", this->crdt}};
        }

        static send_data from_json(const json& j){
            send_data d;
            d.id = j.at("id").get<std::string>();
            d.crdt = j.at("crdt").get<std::string>();

            return d;
        }
    };

    //void send_data_from_json(exchangable_data::send_data& d,const json& j) {
    //    d.id = j.at("id").get<std::string>();
    //    d.crdt = j.at("crdt").get<std::string>();
    //}
}



#endif // TRANSLATOR_EXCHANGEABLE_H
