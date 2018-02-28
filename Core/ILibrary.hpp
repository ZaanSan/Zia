//
// Created by Corentin on 28/01/2018.
//

#ifndef ILIBRARY_HPP
#define ILIBRARY_HPP

#ifdef WIN32
# define LIB_EXTENSION ".dll"
# define LIB_PREFIX ""
#else
# define LIB_EXTENSION ".so"
# define LIB_PREFIX "lib"
#endif

#include "../api/net.h"
#include "../api/conf.h"
#include "../api/module.h"
#include "ZiaException.hpp"

class ILibrary
{

 public:
  virtual ~ILibrary() = default;

    virtual zia::api::Net *LoadNet(const std::string &pathlib) = 0;
    virtual zia::api::Module *LoadModule(const std::string &pathlib) = 0;
};


#endif //R_TYPE_ILIBRARY_HPP
