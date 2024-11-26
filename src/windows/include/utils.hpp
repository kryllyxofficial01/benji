#ifndef _BENJI_UTILS_HPP
#define _BENJI_UTILS_HPP

#include <string>
#include <map>

typedef std::map<std::string, std::string> json_t;

std::string serialize_json(json_t data);
std::string json_data_to_post_request(json_t data);

#endif