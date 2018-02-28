//
// Created by corentin on 20/02/18.
//

#include "Core.hpp"

//Core Core::_singleton = Core();

Core::Core()
    : _configHandler("zia.conf"), _moduleManager(), _mutex()
{

}

Core::~Core() = default;

void Core::run()
{
    updateConfig();
}

void Core::updateConfig()
{
    zia::api::Conf configuration;

    configuration = _configHandler.getConfiguration();
    _moduleManager.updateModules(configuration, _moduleNet, _pipeline);
}

void Core::callback(zia::api::Net::Raw raw, zia::api::NetInfo info)
{
    _mutex.lock();
    updateConfig();
    std::string string1(reinterpret_cast<const char *>(&raw[0]), raw.size());
    std::cout << "-------- Received Data --------- " << std::endl << string1;

    zia::api::HttpDuplex duplex;
    duplex.raw_req = raw;
    zia::api::Conf conf = _configHandler.getConfiguration();
    for (auto &module : _pipeline)
    {
        module->config(conf);
        module->exec(duplex);
    }
    _moduleNet->send(info.sock, duplex.raw_resp);
    _mutex.unlock();
}

Core &Core::Instance()
{
    static Core _instance;
    return _instance;
}