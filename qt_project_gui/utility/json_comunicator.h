#ifndef JSON_COMUNICATOR_H
#define JSON_COMUNICATOR_H


////#include <nlohmann/json.hpp>
////#include "boost/any.hpp"
//#include "../data_structure/action.h"
//
//using json = nlohmann::json;
//
////namespace json_serializer {
////    std::string sendAction(ActionWrapper &actionWrapper);
////    void receiveAction(json j);
////}
//
//
//    class send_data {
//    public:
//        std::string id;
//        std::string crdt;
//
//        send_data(std::string id,std::string crdt) :id(id),  crdt(crdt) {}
//    };
//
//
//    void to_json(json& j, const send_data& d) {
//        j = json{{"id", d.id}, {"crdt", d.crdt}};
//    }
//
//    void from_json(const json& j, send_data& d) {
//        d.id = j.at("id").get<std::string>();
//        d.crdt = j.at("crdt").get<std::string>();
//    }

//    void to_json(json& j, const ActionWrapper& actionWrapper) {
//        j.push_back(actionWrapper.rel_symbol.first);
//        j.push_back(actionWrapper.rel_symbol.second);
//        j.push_back(actionWrapper.symbol.size());
//        for (auto p : actionWrapper.symbol) {
//            j.push_back(p.first);
//            j.push_back(p.second);
//        }
//
//        const Action &a = actionWrapper.action;
//        ActionType type = a.getActionType();
//        j.push_back(type);
//
//
//        switch(type) {
//            case Insertion:
//                j.push_back(a.getChars().toUtf8().constData());
//                j.push_back(a.getComboFontIndex());
//                j.push_back(a.getFontSize());
//                j.push_back(a.getBold());
//                j.push_back(a.getItalic());
//                j.push_back(a.getUnderlined());
//                j.push_back(a.getBlockFormat());
//
//                break;
//
//            case  TextFormatting:
//                j.push_back(a.getSelection());
//                switch (a.getSelection()) {
//                    case 0:
//                        j.push_back(a.getBold());
//                        break;
//                    case 1:
//                        j.push_back(a.getItalic());
//                        break;
//                    case 2:
//                        j.push_back(a.getUnderlined());
//                        break;
//                    case 3:
//                        j.push_back(a.getComboFontIndex());
//                        break;
//                    case 4:
//                        j.push_back(a.getFontSize());
//                        break;
//                }
//                break;
//            case  BlockFormatting:
//                j.push_back(a.getBlockFormat());
//                break;
//            default:
//                break;
//        }
//    }
//
//    void from_json(const json& j, ActionWrapper& actionWrapper) {
//        std::string s = j.dump();
//        std::pair<int,int> p;
//        int i = 0;
//        p.first = j[i].get<int>();
//        p.second = j[++i].get<int>();
//        int sz = j[++i].get<int>();
//        std::vector<std::pair<int,int>> vc(sz);
//        for (int z = 0; z < sz; ++z) {
//            vc[z].first = j[++i].get<int>();
//            vc[z].second = j[++i].get<int>();
//        }
//
//        int type = j[++i].get<int>();
//        Action a;
//
//        switch(type) {
//            case Insertion:
//                //yikes
//                a = Action(QString::fromStdString(j[i+1].get<std::string>()), j[i+2].get<int>(), j[i+3].get<int>(), j[i+4].get<bool>(), j[i+5].get<bool>(), j[i+6].get<bool>(),
//                           static_cast<BlockFormatType>(j[i+7].get<int>()));
//                break;
//            case  TextFormatting:
//                a = Action(j[i+1].get<int>(), j[i+2].get<int>());
//                break;
//            case  BlockFormatting:
//                a = Action(static_cast<BlockFormatType>(j[i+1].get<int>()));
//                break;
//            default:
//                break;
//        }
//        actionWrapper = ActionWrapper(a, p, vc);
//    }


#endif // JSON_COMUNICATOR_H