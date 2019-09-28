#ifndef JSON_COMUNICATOR_H
#define JSON_COMUNICATOR_H

#include <iostream>
#include <nlohmann/json.hpp>
//#include "boost/any.hpp"
#include "../data_structure/action.h"

using json = nlohmann::json;

namespace json_serializer {
    std::string sendAction(ActionWrapper &actionWrapper);
    void receiveAction(json j);
}
#endif // JSON_COMUNICATOR_H