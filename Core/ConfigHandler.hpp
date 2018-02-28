//
// Created by corentin on 22/02/18.
//

#ifndef CPP_ZIA_CONFIGHANDLER_HPP
#define CPP_ZIA_CONFIGHANDLER_HPP

#include <fstream>
#include <iostream>
#include "../api/conf.h"
#include "../Parser/json.hpp"
#include "ZiaException.hpp"

using Json = nlohmann::json;

class ConfigHandler
{
public:
    explicit ConfigHandler(const std::string &file);
    ~ConfigHandler();
    ConfigHandler(const ConfigHandler &) = delete;
    ConfigHandler &operator=(const ConfigHandler &) = delete;

    zia::api::Conf getConfiguration();

private:
    void    addKeyValueString(zia::api::Conf &conf, const std::string &key, const std::string &value);
    void    addKeyValueTab(zia::api::Conf &conf, const std::string &key, const std::vector<std::string> &value);
    void    addKeyValueBool(zia::api::Conf &conf, const std::string &key, bool value);
    void    addKeyValueNumber(zia::api::Conf &conf, const std::string &key, double value);

private:
    std::string path;
};


#endif //CPP_ZIA_CONFIGHANDLER_HPP
