//
// Created by corentin on 22/02/18.
//

#ifndef CPP_ZIA_REQUESTPARSER_HPP
#define CPP_ZIA_REQUESTPARSER_HPP

#include <iostream>
#include "../../api/module.h"

class RequestParser : public zia::api::Module
{
private:
    std::vector<std::string>    _allLines;

public:
    RequestParser();
    ~RequestParser() override;
    RequestParser(const RequestParser &) = delete;
    RequestParser   &operator=(const RequestParser &) = delete;
    bool    config(const zia::api::Conf& conf);
    bool    exec(zia::api::HttpDuplex& htpp);

private:
    void    getAllLines(const zia::api::Net::Raw &request);
    bool    parseLines(zia::api::HttpDuplex& http);
    bool    parseRequest(const std::string &head, zia::api::HttpDuplex& http);
    bool    parseHeader(const std::string &head, zia::api::HttpDuplex& http);
    zia::api::http::Version getVersion(const std::string &version);
    zia::api::http::Method getMethod(const std::string &version);
    void    dump(zia::api::HttpDuplex &http);
};


#endif //CPP_ZIA_REQUESTPARSER_HPP
