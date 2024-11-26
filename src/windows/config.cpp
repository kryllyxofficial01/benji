#include "include/config.hpp"

toml::table parse_config_file(std::string path) {
    toml::table data;

    try {
        data = toml::parse_file(path);

        std::cout << data << std::endl;
    }
    catch (const toml::parse_error& error) {
        std::cerr << error << std::endl;
    }

    return data;
}