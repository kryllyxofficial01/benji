#include "include/utils.hpp"

#include <iostream>

std::string serialize_map(std::map<std::string, std::string> data) {
    std::string serialized = "{";

    for (auto element: data) {
        char* item;
        sprintf(item, "\"%s\": \"%s\",", element.first.c_str(), element.second.c_str());

        serialized += item;
    }

    serialized.at(serialized.length() - 1) = '}';

    return serialized;
}