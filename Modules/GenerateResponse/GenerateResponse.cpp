//
// Created by corentin on 23/02/18.
//

#include <algorithm>
#include "GenerateResponse.hpp"

GenerateResponse::GenerateResponse() = default;

GenerateResponse::~GenerateResponse() = default;

bool GenerateResponse::exec(zia::api::HttpDuplex &http)
{
    if (http.resp.status != zia::api::http::common_status::ok)
        generateAutoResponse(http);
    generateVersion(http);
    generateStatus(http);
    generateHeader(http);
    generateBody(http);
    dump(http);
    return true;
}

bool GenerateResponse::config(const zia::api::Conf &conf)
{
    return true;
}

void GenerateResponse::generateBody(zia::api::HttpDuplex &http)
{
    for (auto &byte : http.resp.body)
    {
        http.raw_resp.push_back(byte);
    }
}

void GenerateResponse::generateVersion(zia::api::HttpDuplex &http)
{
    std::string version;

    if (http.resp.version == zia::api::http::Version::http_0_9)
        version = "HTTP/0.9";
    else if (http.resp.version == zia::api::http::Version::http_1_0)
        version = "HTTP/1.0";
    else if (http.resp.version == zia::api::http::Version::http_1_1)
        version = "HTTP/1.1";
    else if (http.resp.version == zia::api::http::Version::http_2_0)
        version = "HTTP/2";

    std::transform(version.begin(), version.end(), std::back_inserter(http.raw_resp),
                   [](unsigned char c) { return std::byte(c); });
}

void GenerateResponse::generateStatus(zia::api::HttpDuplex &http)
{
    std::string status;

    if (http.resp.status == zia::api::http::common_status::ok)
        status = std::string(" 200 OK") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::created)
        status = std::string(" 201 Created") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::accepted)
        status = std::string(" 202 Accepted") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::no_content)
        status = std::string(" 204 No Content") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::multiple_choices)
        status = std::string(" 300 Multiple Choices") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::moved_permanently)
        status = std::string(" 301 Moved Permanently") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::not_modified)
        status = std::string(" 304 Not Modified") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::bad_request)
        status = std::string(" 400 Bad Request") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::unauthorized)
        status = std::string(" 401 Unauthorized") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::forbidden)
        status = std::string(" 403 Forbidden") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::not_found)
        status = std::string(" 404 Not Found") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::method_not_allowed)
        status = std::string(" 405 Method Not Allowed") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::internal_server_error)
        status = std::string(" 500 Internal Server Error") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::not_implemented)
        status = std::string(" 501 Not Implemented") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::bad_gateway)
        status = std::string(" 502 Bad Gateway") + "\r\n";
    else if (http.resp.status == zia::api::http::common_status::service_unavailable)
        status = std::string(" 503 Service Unavailable") + "\r\n";

    std::transform(status.begin(), status.end(), std::back_inserter(http.raw_resp),
                   [](unsigned char c) { return std::byte(c); });
}

void GenerateResponse::generateHeader(zia::api::HttpDuplex &http)
{
    std::string header;
    for (auto &head : http.resp.headers)
    {
        header = head.first + ": " + head.second + "\n";
        std::transform(header.begin(), header.end(), std::back_inserter(http.raw_resp),
                       [](unsigned char c) { return std::byte(c); });
    }
    http.raw_resp.push_back(static_cast<std::byte >('\n'));
}

void GenerateResponse::dump(zia::api::HttpDuplex &http)
{
    std::cout << "-------- HTTP RESPONSE --------" << std::endl;
    for (auto &byte : http.raw_resp)
        std::cout << static_cast<char>(byte);
}

std::string GenerateResponse::autoResponse(const std::string &title, const std::string &h1) {
    return std::string("<html>\n"
                               "<head><title>" + title + "</title></head>\n"
                               "<body><h1>" + h1 + "</h1></body>\n"
                               "</html>\n");
}

void GenerateResponse::generateAutoResponse(zia::api::HttpDuplex &http)
{
    std::string status;
    http.resp.body.clear();
    status = "<!DOCTYPE html>\n";
    if (http.resp.status == zia::api::http::common_status::created)
        status += autoResponse("Created", "201 Created");
    else if (http.resp.status == zia::api::http::common_status::accepted)
        status += autoResponse("Accepted", "202 Accepted");
    else if (http.resp.status == zia::api::http::common_status::no_content)
        status += autoResponse("No Content", "204 No Content");
    else if (http.resp.status == zia::api::http::common_status::multiple_choices)
        status += autoResponse("Multiple Choices", "300 Multiple Choices");
    else if (http.resp.status == zia::api::http::common_status::moved_permanently)
        status += autoResponse("Moved Permanently", "301 Moved Permanently");
    else if (http.resp.status == zia::api::http::common_status::not_modified)
        status += autoResponse("Not Modified", "304 Not Modified");
    else if (http.resp.status == zia::api::http::common_status::bad_request)
        status += autoResponse("Bad Request", "400 Bad Request");
    else if (http.resp.status == zia::api::http::common_status::unauthorized)
        status += autoResponse("Unauthorized", "401 Unauthorized");
    else if (http.resp.status == zia::api::http::common_status::forbidden)
        status += autoResponse("Forbidden", "403 Forbidden");
    else if (http.resp.status == zia::api::http::common_status::not_found)
        status += autoResponse("Not Found", "404 Not Found");
    else if (http.resp.status == zia::api::http::common_status::method_not_allowed)
        status += autoResponse("Not Allowed", "405 Not Allowed");
    else if (http.resp.status == zia::api::http::common_status::internal_server_error)
        status += autoResponse("Internal Server Error", "500 Internal Server Error");
    else if (http.resp.status == zia::api::http::common_status::not_implemented)
        status += autoResponse("Not Implemented", "501 Not Implemented");
    else if (http.resp.status == zia::api::http::common_status::bad_gateway)
        status += autoResponse("Bad Gateway", "502 Bad Gateway");
    else if (http.resp.status == zia::api::http::common_status::service_unavailable)
        status += autoResponse("Service Unavailable", "503 Service Unavailable");
    http.resp.headers["content-length"] = std::to_string(status.size());
    std::transform(status.begin(), status.end(), std::back_inserter(http.resp.body),
                   [](unsigned char c) { return std::byte(c); });
}

#ifndef WIN32
extern "C"
{
zia::api::Module *create()
{
    return new GenerateResponse();
}
};
#else
extern "C" {
zia::api::Module __declspec(dllexport) *create()
{
    return new GenerateResponse();
}
};
#endif //ifndef WIN32