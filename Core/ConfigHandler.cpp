//
// Created by corentin on 22/02/18.
//

#include "ConfigHandler.hpp"

ConfigHandler::ConfigHandler(const std::string &file)
{
    #ifndef WIN32
      this->path = "../Conf/" + file;
    #else
      this->path = "..\\Conf\\" + file;
    #endif
}

ConfigHandler::~ConfigHandler() = default;

zia::api::Conf ConfigHandler::getConfiguration()
{
    zia::api::Conf  configuration;
    Json            parser;
    std::ifstream   i(path);

    if (i.is_open())
    {
        i >> parser;
        i.close();
    }
    else
    {
        throw ZiaException("Can't open file : " + path);
    }
    for (auto it = parser["zia_conf"].begin(); it != parser["zia_conf"].end(); ++it)
    {
        if (it.value().is_string())
        {
            addKeyValueString(configuration, it.key(), it.value());
        }
        else if (it.value().is_number())
        {
            addKeyValueNumber(configuration, it.key(), it.value());
        }
        else if (it.value().is_array())
        {
            addKeyValueTab(configuration, it.key(), it.value());
        }
        else if (it.value().is_boolean())
        {
            addKeyValueBool(configuration, it.key(), it.value());
        }
    }
    return configuration;
}

void ConfigHandler::addKeyValueString(zia::api::Conf &conf,
                                const std::string &key,
                                const std::string &value)
{
    zia::api::ConfValue val;

    val.v = value;
    if (!conf.insert(std::make_pair(key, val)).second)
        conf[key] = val;
}

void ConfigHandler::addKeyValueTab(zia::api::Conf &conf, const std::string &key, const std::vector<std::string> &value)
{
    zia::api::ConfValue val;
    zia::api::ConfArray array;

    for (const auto &it : value)
    {
        zia::api::ConfValue newVal;
        newVal.v = it;
        array.push_back(newVal);
    }
    val.v = array;
    if (!conf.insert(std::make_pair(key, val)).second)
        conf[key] = val;
}

void ConfigHandler::addKeyValueNumber(zia::api::Conf &conf, const std::string &key, double value)
{
    zia::api::ConfValue val;

    if (std::fmod(value, 1) == 0)
    {
        // Int
        val.v = static_cast<long long>(value);
    }
    else
    {
        // Double
        val.v = value;
    }
    if (!conf.insert(std::make_pair(key, val)).second)
        conf[key] = val;
}

void ConfigHandler::addKeyValueBool(zia::api::Conf &conf, const std::string &key, bool value)
{
    zia::api::ConfValue val;

    val.v = value;
    if (!conf.insert(std::make_pair(key, val)).second)
        conf[key] = val;
}
