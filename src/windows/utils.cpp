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

std::string json_data_to_post_request(JSON data) {
    std::string data_string = serialize_json(data);

    std::string request = "POST / HTTP/1.1\r\n";
    request += "Host: localhost:8080\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: " + std::to_string(data_string.length()) + "\r\n";
    request += "Connection: keep-alive\r\n\r\n";
    request += data_string;

    return request;
}