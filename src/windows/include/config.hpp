#ifndef _BENJI_CONFIG_HPP
#define _BENJI_CONFIG_HPP

#include <iostream>
#include <string>

#include <toml++/toml.hpp>

toml::table parse_config_file(std::string path);

#endif