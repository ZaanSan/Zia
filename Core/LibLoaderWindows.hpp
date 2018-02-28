//
// Created by Corentin on 28/01/2018.
//

#ifndef LIBLOADERWINDOWS_HPP
#define LIBLOADERWINDOWS_HPP

# ifdef WIN32

# include <windows.h>
# include "ILibrary.hpp"

typedef zia::api::Net *(*net_ptr)();
typedef zia::api::Module *(*module_ptr)();

class LibLoaderWindows : public ILibrary
{
public:
    LibLoaderWindows() = default;
    ~LibLoaderWindows() = default;
    virtual zia::api::Net *LoadNet(const std::string &pathlib);
    virtual zia::api::Module *LoadModule(const std::string &pathlib);
};

# endif //WIN32
#endif //LIBLOADERWINDOWS_HPP
