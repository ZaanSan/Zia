#ifndef CPP_ZIA_PHP_HPP
# define CPP_ZIA_PHP_HPP

#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>
#include "../../api/module.h"

class Php : public zia::api::Module
{
  public:
    Php() = default;
    ~Php() override = default;
    Php(const Php &) = delete;
    Php &operator=(const Php &) = delete;
    bool    config(const zia::api::Conf &conf) override;
    bool    exec(zia::api::HttpDuplex &http) override;

  protected:
    std::string ExecCmd(const char *cmd, zia::api::HttpDuplex &http);
    bool FillBody(zia::api::HttpDuplex &http);
    bool NotFound(zia::api::HttpDuplex &http);
    bool CheckExtension(zia::api::HttpDuplex &http);
};

#endif
