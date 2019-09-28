
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

<<<<<<< HEAD
    //void to_json(json& j, const send_data& d) {
        //j = json{{"id", d.id}, {"crdt", d.crdt}};
        //}

    //void from_json(const json& j, send_data& d) {
=======
            return d;
        }
    };

    //void send_data_from_json(exchangable_data::send_data& d,const json& j) {
>>>>>>> 882f64da1e08511a28fc57f4ebdad5048208435b
    //    d.id = j.at("id").get<std::string>();
    //    d.crdt = j.at("crdt").get<std::string>();
    //}
}



#endif // TRANSLATOR_EXCHANGEABLE_H
