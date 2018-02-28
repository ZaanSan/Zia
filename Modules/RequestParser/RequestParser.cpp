//
// Created by corentin on 22/02/18.
//

#include "RequestParser.hpp"

//TODO Améliorer le parsing (vérifier les headers..)

RequestParser::RequestParser() = default;

RequestParser::~RequestParser() = default;

bool RequestParser::config(const zia::api::Conf &conf)
{
    return true;
}

bool RequestParser::exec(zia::api::HttpDuplex &http)
{
    getAllLines(http.raw_req);
    if (!parseLines(http))
    {
        _allLines.clear();
        return false;
    }
    _allLines.clear();
    dump(http);
    return true;
}

void     RequestParser::getAllLines(const zia::api::Net::Raw &request)
{
    std::string line;
    char car;

    for (auto &c : request)
    {
        car = static_cast<char>(c);
        if (car == 0)
            break;
        else if (car == '\n')
        {
            if (line.find('\r') != std::string::npos)
                line.pop_back();
            _allLines.push_back(line);
            line.clear();
        }
        else
        {
            line += car;
        }
    }
}

bool RequestParser::parseLines(zia::api::HttpDuplex& http)
{
    int i = 0;
    for (auto &line : _allLines)
    {
        if (i == 0)
        {
            if (!parseRequest(line, http))
                return false;
        }
        else
        {
            if (!line.empty())
            {
                if (!parseHeader(line, http))
                    return false;
            }
        }
        i++;
    }
    return true;
}

bool RequestParser::parseRequest(const std::string &head, zia::api::HttpDuplex& http)
{
    std::string toParse = head;
    std::vector<std::string> request;
    size_t pos = 0;
    std::string token;

    while ((pos = toParse.find(' ')) != std::string::npos)
    {
        token = toParse.substr(0, pos);
        request.push_back(token);
        toParse.erase(0, pos + 1);
    }
    if (!toParse.empty())
        request.push_back(toParse);
    if (request.size() != 3)
    {
        std::cout << "Wrong request" << std::endl;
        return false;
    }
    http.req.version = getVersion(request[2]);
    http.req.method = getMethod(request[0]);
    http.req.uri = request[1];
    return true;
}

bool RequestParser::parseHeader(const std::string &head, zia::api::HttpDuplex& http)
{
    std::string toParse = head;
    std::vector<std::string> request;
    size_t pos = 0;
    std::string token;

    while ((pos = toParse.find(": ")) != std::string::npos)
    {
        token = toParse.substr(0, pos);
        request.push_back(token);
        toParse.erase(0, pos + 2);
    }
    if (!toParse.empty())
        request.push_back(toParse);
    if (request.size() != 2)
    {
        std::cout << "Wrong request header" << std::endl;
        return false;
    }
    http.req.headers.insert(std::make_pair(request[0], request[1]));
    return true;
}

zia::api::http::Version RequestParser::getVersion(const std::string &version)
{
    if (version == "HTTP/0.9")
        return zia::api::http::Version::http_0_9;
    else if (version == "HTTP/1.0")
        return zia::api::http::Version::http_1_0;
    else if (version == "HTTP/1.1")
        return zia::api::http::Version::http_1_1;
    else if (version == "HTTP/2.0" ||
             version == "HTTP/2")
        return zia::api::http::Version::http_2_0;
    else
    {
        std::cout << "Wrong Http Verison : " + version << std::endl;
        return zia::api::http::Version::unknown;
    }
}

zia::api::http::Method RequestParser::getMethod(const std::string &version)
{
    if (version == "GET")
        return zia::api::http::Method::get;
    else if (version == "POST")
        return zia::api::http::Method::post;
    else if (version == "HEAD")
        return zia::api::http::Method::head;
    else if (version == "PUT")
        return zia::api::http::Method::put;
    else if (version == "DELETE")
        return zia::api::http::Method::delete_;
    else if (version == "CONNECT")
        return zia::api::http::Method::connect;
    else if (version == "OPTIONS")
        return zia::api::http::Method::options;
    else if (version == "TRACE")
        return zia::api::http::Method::trace;
    else
    {
        std::cout << "Wrong HTTP method : " + version << std::endl;
        return zia::api::http::Method::unknown;
    }
}

void RequestParser::dump(zia::api::HttpDuplex &http)
{
    std::cout << "----- AFTER PARSING ----" << std::endl;
    if (http.req.version == zia::api::http::Version::http_1_1)
        std::cout << "Version : HTTP 1.1" << std::endl;
    std::cout << "Uri : " << http.req.uri << std::endl;
    if (http.req.method == zia::api::http::Method::get)
        std::cout << "Method : GET" << std::endl;
    else if (http.req.method == zia::api::http::Method::post)
        std::cout << "Method : POST" << std::endl;
    for (auto &val : http.req.headers)
    {
        std::cout << "Header : " << val.first << " => " << val.second << std::endl;
    }
}

#ifndef WIN32
extern "C"
{
zia::api::Module *create()
{
    return new RequestParser();
}
};
#else
extern "C" {
zia::api::Module __declspec(dllexport) *create()
{
    return new RequestParser();
}
};
#endif //ifndef WIN32