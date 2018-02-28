//
// Created by corentin on 23/02/18.
//

#ifndef CPP_ZIA_GENERATERESPONSE_HPP
#define CPP_ZIA_GENERATERESPONSE_HPP

#include <iostream>
#include "../../api/module.h"

class GenerateResponse : public zia::api::Module
{
public:
    GenerateResponse();
    ~GenerateResponse() override;
    GenerateResponse(const GenerateResponse &) = delete;
    GenerateResponse &operator=(const GenerateResponse &) = delete;
    bool    config(const zia::api::Conf &conf);
    bool    exec(zia::api::HttpDuplex &http);

private:
    void    generateVersion(zia::api::HttpDuplex &http);
    void    generateStatus(zia::api::HttpDuplex &http);
    void    generateHeader(zia::api::HttpDuplex &http);
    void    generateBody(zia::api::HttpDuplex &http);
    void    generateAutoResponse(zia::api::HttpDuplex &http);
    void    dump(zia::api::HttpDuplex &http);
    std::string autoResponse(const std::string &title, const std::string &h1);
};


#endif //CPP_ZIA_GENERATERESPONSE_HPP
