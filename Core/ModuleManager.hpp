//
// Created by corentin on 23/02/18.
//

#ifndef CPP_ZIA_MODULEMANAGER_HPP
#define CPP_ZIA_MODULEMANAGER_HPP

#include <memory>
#include <list>
#include "../api/net.h"
#include "../api/module.h"
#include "../api/conf.h"
#include "LibLoader.hpp"
#include "LibLoaderWindows.hpp"

class ModuleManager
{
private:
    std::unique_ptr<ILibrary>                       _libLoader;
    std::string                                     _prevNetModule;
    std::string                                     _nextNetModule;
    std::vector<std::string>                        _prevModules;
    std::vector<std::string>                        _nextModules;
public:
    ModuleManager();
    ~ModuleManager();
    ModuleManager(const ModuleManager &) = delete;
    ModuleManager &operator=(const ModuleManager &) = delete;

    void    updateModules(const zia::api::Conf &conf,
                          std::shared_ptr<zia::api::Net> &moduleNet,
                          std::list<std::shared_ptr<zia::api::Module>> &pipeline);

private:
    void    loadNetModule(const zia::api::Conf &conf, std::shared_ptr<zia::api::Net> &moduleNet);
    void    loadAllModule(const zia::api::Conf &conf, std::list<std::shared_ptr<zia::api::Module>> &pipeline);
    void    getNextNetModule(const zia::api::Conf &conf);
    void    getNextModules(const zia::api::Conf &conf);
    bool    compareNetModule();
    bool    compareModules();
    void    reloadNetModule(std::shared_ptr<zia::api::Net> &moduleNet);
    void    reloadModules(std::list<std::shared_ptr<zia::api::Module>> &pipeline);
};


#endif //CPP_ZIA_MODULEMANAGER_HPP
