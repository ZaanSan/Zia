#pragma once

#ifndef WIN32
# include <dlfcn.h>
# include "ILibrary.hpp"

class LibLoader : public ILibrary
{
 public:
  LibLoader();

  ~LibLoader() override;

    virtual zia::api::Net *LoadNet(const std::string &pathlib);
    virtual zia::api::Module *LoadModule(const std::string &pathlib);

};

typedef zia::api::Net *create_net();
typedef zia::api::Module *create_module();

#endif