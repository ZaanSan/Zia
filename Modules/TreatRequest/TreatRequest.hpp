//
// Created by corentin on 23/02/18.
//

#ifndef CPP_ZIA_TREATREQUEST_HPP
#define CPP_ZIA_TREATREQUEST_HPP

#include "../../api/module.h"

class TreatRequest : public zia::api::Module
{
public:
    TreatRequest();
    ~TreatRequest() override;
    TreatRequest(const TreatRequest &) = delete;
    TreatRequest &operator=(const TreatRequest &) = delete;
    bool    config(const zia::api::Conf &conf) override;
    bool    exec(zia::api::HttpDuplex &http) override;

private:
    bool    treatReq(zia::api::HttpDuplex &http);
    bool    badRequest(zia::api::HttpDuplex &http);
    bool    NotFound(zia::api::HttpDuplex &http);
    bool    methodNotAllowed(zia::api::HttpDuplex &http);
    bool    urlDecode(zia::api::HttpDuplex &httpDuplex);
    void    setContentType(zia::api::HttpDuplex &httpDuplex, const std::string &extension);
};


#endif //CPP_ZIA_TREATREQUEST_HPP
