#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <string>
#include <map>

#define JSON std::map<std::string, std::string>

std::string serialize_json(JSON data);

#endif