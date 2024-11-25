#ifndef _BENJI_UTILS_HPP
#define _BENJI_UTILS_HPP

#include <string>
#include <map>

#define JSON std::map<std::string, std::string>

std::string serialize_json(JSON data);
std::string json_data_to_post_request(JSON data);

#endif