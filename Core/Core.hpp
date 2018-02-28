//
// Created by corentin on 20/02/18.
//

#ifndef CPP_ZIA_CORE_HPP
#define CPP_ZIA_CORE_HPP

#include <list>
#include <mutex>
#include "ZiaException.hpp"
#include "ConfigHandler.hpp"
#include "LibLoader.hpp"
#include "LibLoaderWindows.hpp"
#include "ModuleManager.hpp"

class Core
{
private:
    ConfigHandler                                   _configHandler;
    ModuleManager                                   _moduleManager;
    std::shared_ptr<zia::api::Net>                  _moduleNet;
    std::list<std::shared_ptr<zia::api::Module>>    _pipeline;
    //static Core _singleton;
    std::mutex  _mutex;

public:
    static Core &Instance();
    Core(const Core &) = delete;
    Core    &operator=(const Core &) = delete;
    void    run();
    void    updateConfig();
    void    callback(zia::api::Net::Raw raw, zia::api::NetInfo info);

private:
    Core();
    ~Core();
};


#endif //CPP_ZIA_CORE_HPP
