//
// Created by Corentin on 28/01/2018.
//

#ifdef WIN32

#include <iostream>
#include "LibLoaderWindows.hpp"

zia::api::Net *LibLoaderWindows::LoadNet(const std::string &pathlib)
{
    HMODULE myDLL = LoadLibrary(pathlib.c_str());
    if (!myDLL)
    {
        std::cout << GetLastError() << std::endl;
        throw ZiaException("Can't find Library:" + pathlib);
    }
    net_ptr function = (net_ptr) GetProcAddress(myDLL, "create");
    if (!function)
    {
            throw ZiaException("Can't load symbol create");
    }
    auto ia = function();
    return (ia);
}

zia::api::Module *LibLoaderWindows::LoadModule(const std::string &pathlib)
{
    SetErrorMode(static_cast<UINT>(true));
    HMODULE myDLL = LoadLibrary(pathlib.c_str());
    if (!myDLL)
    {
        std::cout << GetLastError() << std::endl;
        throw ZiaException("Can't find Library:" + pathlib);
    }
    module_ptr function = (module_ptr) GetProcAddress(myDLL, "create");
    if (!function)
    {
            throw ZiaException("Can't load symbol create");
    }
    auto ia = function();
    return (ia);
}

#endif
