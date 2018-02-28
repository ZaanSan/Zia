//
// Created by corentin on 23/02/18.
//

#include <iostream>
#include "ModuleManager.hpp"
#include "Core.hpp"
#include "ZiaException.hpp"

ModuleManager::ModuleManager()
{
#ifndef WIN32
    _libLoader = std::make_unique<LibLoader>();
#else
    _libLoader = std::make_unique<LibLoaderWindows>();
#endif // ifndef WIN32
}

ModuleManager::~ModuleManager() = default;

void    callbackToCore(zia::api::Net::Raw raw, zia::api::NetInfo info)
{
    Core &core = Core::Instance();

    core.callback(std::move(raw), std::move(info));
}

void ModuleManager::updateModules(const zia::api::Conf &conf,
                                  std::shared_ptr<zia::api::Net> &moduleNet,
                                  std::list<std::shared_ptr<zia::api::Module>> &pipeline)
{
    if (_prevModules.empty() && _prevNetModule.empty())
    {
        std::cout << "Modules loading..." << std::endl;
        loadNetModule(conf, moduleNet);
        loadAllModule(conf, pipeline);
        moduleNet->config(conf);
        moduleNet->run(&callbackToCore);
    }
    else
    {
        std::cout << "Modules already load" << std::endl;
        getNextNetModule(conf);
        getNextModules(conf);
        if (!compareNetModule())
           {
             moduleNet->stop();
             reloadNetModule(moduleNet);
             moduleNet->config(conf);
             moduleNet->run(&callbackToCore);
           }
        if (!compareModules())
            reloadModules(pipeline);
        _prevModules = _nextModules;
        _prevNetModule = _nextNetModule;
        _nextModules.clear();
        _nextNetModule.clear();
    }
}

void ModuleManager::loadNetModule(const zia::api::Conf &conf, std::shared_ptr<zia::api::Net> &moduleNet)
{
    #ifndef WIN32
      std::string libPath = "../";
    #else
      std::string libPath = "..\\";
    #endif
    std::string libDir;
    std::string libName;
    zia::api::ConfArray array;
    zia::api::ConfValue valuePath;
    zia::api::ConfValue valueName;

    valuePath = conf.at("modules_path");
    valueName = conf.at("module_net");

    array = std::get<zia::api::ConfArray>(valuePath.v);
    libDir = std::get<std::string>(array[0].v);
    libName = LIB_PREFIX + std::get<std::string>(valueName.v) + LIB_EXTENSION;

    if (libDir.empty())
        throw ZiaException("Can't find modules_path configuration");
    else if (libName.empty())
        throw ZiaException("Can't find module_net configuration");
    #ifndef WIN32
      libPath += libDir + "/" + libName;
    #else
      libPath += libDir + "\\" + libName;
    #endif
    _prevNetModule = libPath;
    moduleNet = std::shared_ptr<zia::api::Net>(_libLoader->LoadNet(libPath));
}

void ModuleManager::loadAllModule(const zia::api::Conf &conf, std::list<std::shared_ptr<zia::api::Module>> &pipeline)
{
    std::string libPath;
    std::string libDir;
    std::string libName;
    zia::api::ConfArray array;
    zia::api::ConfArray arrayModule;
    zia::api::ConfValue valuePath;
    zia::api::ConfValue valueModules;

    valuePath = conf.at("modules_path");
    valueModules = conf.at("modules");

    array = std::get<zia::api::ConfArray>(valuePath.v);
    libDir = std::get<std::string>(array[0].v);

    arrayModule = std::get<zia::api::ConfArray>(valueModules.v);
    for (auto &it : arrayModule)
    {
        #ifndef WIN32
          libPath = "../" + libDir + "/" + LIB_PREFIX + std::get<std::string>(it.v) + LIB_EXTENSION;
        #else
          libPath = "..\\" + libDir + "\\" + LIB_PREFIX + std::get<std::string>(it.v) + LIB_EXTENSION;
        #endif
        _prevModules.push_back(libPath);
        pipeline.push_back(std::shared_ptr<zia::api::Module>(_libLoader->LoadModule(libPath)));
    }
}

void ModuleManager::getNextNetModule(const zia::api::Conf &conf)
{
    #ifndef WIN32
      std::string libPath = "../";
    #else
      std::string libPath = "..\\";
    #endif
    std::string libDir;
    std::string libName;
    zia::api::ConfArray array;
    zia::api::ConfValue valuePath;
    zia::api::ConfValue valueName;

    valuePath = conf.at("modules_path");
    valueName = conf.at("module_net");

    array = std::get<zia::api::ConfArray>(valuePath.v);
    libDir = std::get<std::string>(array[0].v);
    libName = LIB_PREFIX + std::get<std::string>(valueName.v) + LIB_EXTENSION;

    if (libDir.empty())
        throw ZiaException("Can't find modules_path configuration");
    else if (libName.empty())
        throw ZiaException("Can't find module_net configuration");
    #ifndef WIN32
      libPath += libDir + "/" + libName;
    #else
      libPath += libDir + "\\" + libName;
    #endif
    _nextNetModule = libPath;
}

void ModuleManager::getNextModules(const zia::api::Conf &conf)
{
    std::string libPath;
    std::string libDir;
    std::string libName;
    zia::api::ConfArray array;
    zia::api::ConfArray arrayModule;
    zia::api::ConfValue valuePath;
    zia::api::ConfValue valueModules;

    valuePath = conf.at("modules_path");
    valueModules = conf.at("modules");

    array = std::get<zia::api::ConfArray>(valuePath.v);
    libDir = std::get<std::string>(array[0].v);

    arrayModule = std::get<zia::api::ConfArray>(valueModules.v);
    for (auto &it : arrayModule)
    {
        #ifndef WIN32
          libPath = "../" + libDir + "/" + LIB_PREFIX + std::get<std::string>(it.v) + LIB_EXTENSION;
        #else
          libPath = "..\\" + libDir + "\\" + LIB_PREFIX + std::get<std::string>(it.v) + LIB_EXTENSION;
        #endif
        _nextModules.push_back(libPath);
    }
}

bool ModuleManager::compareNetModule()
{
    return (_prevNetModule == _nextNetModule);
}

bool ModuleManager::compareModules()
{
    for (unsigned int i = 0; i < _nextModules.size(); i++)
    {
        if (i >= _prevModules.size() ||
                _nextModules[i] != _prevModules[i])
            return false;
    }
    return (_prevModules.size() == _nextModules.size());
}

void    ModuleManager::reloadNetModule(std::shared_ptr<zia::api::Net> &moduleNet)
{
    moduleNet = std::shared_ptr<zia::api::Net>(_libLoader->LoadNet(_nextNetModule));
}

void    ModuleManager::reloadModules(std::list<std::shared_ptr<zia::api::Module>> &pipeline)
{
    pipeline.clear();
    for (auto &module : _nextModules)
        pipeline.push_back(std::shared_ptr<zia::api::Module>(_libLoader->LoadModule(module)));
}
