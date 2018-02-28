#ifndef WIN32

#include <iostream>
#include "LibLoader.hpp"

LibLoader::LibLoader()
= default;

LibLoader::~LibLoader()
= default;

zia::api::Net *LibLoader::LoadNet(const std::string &pathlib)
{
    char *errstr;

    void *handle = dlopen(pathlib.c_str(), RTLD_LAZY);
    if (!handle)
    {
        errstr = dlerror();
        if (errstr != nullptr)
            throw ZiaException("A dynamic linking error occurred: " + std::string(errstr));
        throw ZiaException("Can't find Library: " + pathlib);
    }
    dlerror();

    auto *create = (create_net *) dlsym(handle, "create");
    char *dlsym_error = dlerror();
    if (dlsym_error)
    {
        throw ZiaException("A dynamic linking error occurred: " + std::string(dlsym_error));
    }
    auto l = create();
    return (l);
}

zia::api::Module *LibLoader::LoadModule(const std::string &pathlib)
{
    char *errstr;

    void *handle = dlopen(pathlib.c_str(), RTLD_LAZY);
    if (!handle)
    {
        errstr = dlerror();
        if (errstr != nullptr)
            throw ZiaException("A dynamic linking error occurred: " + std::string(errstr));
        throw ZiaException("Can't find Library:" + pathlib);
    }
    auto *create = (create_module *) dlsym(handle, "create");
    char *dlsym_error = dlerror();
    if (dlsym_error)
    {
        throw ZiaException("HEEA dynamic linking error occurred: " + std::string(dlsym_error));
    }
    auto l = create();
    return (l);
}

#endif
