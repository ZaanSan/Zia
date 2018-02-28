//
// Created by corentin on 23/02/18.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "TreatRequest.hpp"

TreatRequest::TreatRequest() = default;

TreatRequest::~TreatRequest() = default;

bool TreatRequest::config(const zia::api::Conf &conf)
{
    return true;
}

bool TreatRequest::exec(zia::api::HttpDuplex &http)
{
    return treatReq(http);
}

bool TreatRequest::treatReq(zia::api::HttpDuplex &http)
{
    std::string     param;
    size_t          pos;

    http.resp.version = http.req.version;
    // Check Method
    if (http.req.method == zia::api::http::Method::unknown)
    {
        return methodNotAllowed(http);
    }
    // Decoding url (replace + with space ...)
    if (!urlDecode(http))
    {
        return badRequest(http);
    }
    // If request contain parameters
    if ((pos = http.req.uri.find('?')) != std::string::npos)
    {
        param = http.req.uri.substr(pos + 1, http.req.uri.size() - pos - 1);
        http.req.uri = http.req.uri.substr(0, pos);
    }
    // Request path must be absolute and not contain ".."
    if (http.req.uri.empty() || http.req.uri[0] != '/' ||
        http.req.uri.find("..") != std::string::npos)
    {
        return badRequest(http);
    }

    // If path ends in slash => add index.html
    if (http.req.uri[http.req.uri.size() - 1] == '/')
    {
        http.req.uri += "index.html";
    }

    // Determine the file extension.
    std::size_t last_slash_pos =  http.req.uri.find_last_of("/");
    std::size_t last_dot_pos =  http.req.uri.find_last_of(".");
    std::string extension;
    if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
    {
        extension =  http.req.uri.substr(last_dot_pos + 1);
    }
    else
        extension = "";

#ifdef WIN32
    http.req.uri.erase(0, 1);
#endif
    // Open file to send back_insert
    std::ifstream   file(http.req.uri);
    std::string line;
    int length = 0;

    if (file.is_open())
    {
        // If file open

        http.resp.status = zia::api::http::common_status::ok;
        while (std::getline(file, line))
        {
            line += "\n";
            length += line.size();
            std::transform(line.begin(), line.end(), std::back_inserter(http.resp.body),
                           [](unsigned char c) { return std::byte(c); });
        }
    }
    else
    {
        // If no file found
        return NotFound(http);
    }
    setContentType(http, extension);
    http.resp.headers.insert(std::make_pair("content-length", std::to_string(length)));
    http.resp.headers.insert(std::make_pair("server", "Zia 1.0"));
    return true;
}

bool    TreatRequest::urlDecode(zia::api::HttpDuplex &httpDuplex)
{
    std::string decode;
    decode.reserve(httpDuplex.req.uri.size());
    for (std::size_t i = 0; i < httpDuplex.req.uri.size(); ++i)
    {
        if (httpDuplex.req.uri[i] == '%')
        {
            if (i + 3 <= httpDuplex.req.uri.size())
            {
                int value = 0;
                std::istringstream is(httpDuplex.req.uri.substr(i + 1, 2));
                if (is >> std::hex >> value)
                {
                    decode += static_cast<char>(value);
                    i += 2;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else if (httpDuplex.req.uri[i] == '+')
        {
            decode += ' ';
        }
        else
        {
            decode += httpDuplex.req.uri[i];
        }
    }
    httpDuplex.req.uri = decode;
    return true;
}

bool    TreatRequest::NotFound(zia::api::HttpDuplex &http)
{
    http.resp.status = zia::api::http::common_status::not_found;
    http.resp.headers.insert(std::make_pair("content-length", std::to_string(0)));
    http.resp.headers.insert(std::make_pair("content-type", "text/html"));
    http.resp.headers.insert(std::make_pair("server", "Zia 1.0"));
    return true;
}

bool    TreatRequest::badRequest(zia::api::HttpDuplex &http)
{
    http.resp.status = zia::api::http::common_status::bad_request;
    http.resp.headers.insert(std::make_pair("content-length", std::to_string(0)));
    http.resp.headers.insert(std::make_pair("content-type", "text/html"));
    http.resp.headers.insert(std::make_pair("server", "Zia 1.0"));
    return true;
}

bool    TreatRequest::methodNotAllowed(zia::api::HttpDuplex &http)
{
    http.resp.status = zia::api::http::common_status::method_not_allowed;
    http.resp.headers.insert(std::make_pair("content-length", std::to_string(0)));
    http.resp.headers.insert(std::make_pair("content-type", "text/html"));
    http.resp.headers.insert(std::make_pair("server", "Zia 1.0"));
    return true;
}

void TreatRequest::setContentType(zia::api::HttpDuplex &http, const std::string &extension)
{
    if (extension == "png")
        http.resp.headers.insert(std::make_pair("content-type", "image/png"));
    else if (extension == "jpg" || extension == "jpeg")
        http.resp.headers.insert(std::make_pair("content-type", "image/jpeg"));
    else if (extension == "gif")
        http.resp.headers.insert(std::make_pair("content-type", "image/jpeg"));
    else if (extension == "html" || extension == "htm")
        http.resp.headers.insert(std::make_pair("content-type", "text/html"));
    else if (extension == "php")
      http.resp.headers.insert(std::make_pair("content-type", "text/html"));
    else if (extension == "css")
        http.resp.headers.insert(std::make_pair("content-type", "text/css"));
    else if (extension == "xml")
        http.resp.headers.insert(std::make_pair("content-type", "text/xml"));
    else if (extension == "json")
        http.resp.headers.insert(std::make_pair("content-type", "application/json"));
    else
        http.resp.headers.insert(std::make_pair("content-type", "text/plain"));
}

#ifndef WIN32
extern "C"
{
zia::api::Module *create()
{
    return new TreatRequest();
}
};
#else
extern "C" {
zia::api::Module __declspec(dllexport) *create()
{
    return new TreatRequest();
}
};
#endif //ifndef WIN32
