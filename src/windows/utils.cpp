#include "include/utils.hpp"

#include <iostream>

std::string serialize_json(JSON data) {
    std::string serialized = "{";

    for (auto element: data) {
        std::string key = "\"" + element.first + "\"";
        std::string value = "\"" + element.second + "\"";

        serialized += key + ": " + value + ",";
    }

    serialized.at(serialized.length() - 1) = '}';

    return serialized;
}