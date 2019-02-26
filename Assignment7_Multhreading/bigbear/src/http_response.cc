#include "http_response.h"
#include <iostream>
#include <cstring>
#include <boost/log/trivial.hpp>

namespace logging = boost::log;

std::string HttpResponse::buildStatus(int status)
{
    std::string httpProtocol = "HTTP/1.1 " + std::to_string(status);
    switch(status){
        case 200: httpProtocol += " OK";
        break;
        case 400: httpProtocol += " Bad Request";
        break;
        case 404: httpProtocol += " File Not Found";
        break;
        case 418: httpProtocol += " I'm a teapot";
        break;
        default:
        BOOST_LOG_TRIVIAL(debug) << "Invalid status number passed: " << status;
        break;
    }
    // TODO: add other status code discribtion
    // TODO: add an enum for return status also?
    return  httpProtocol + "\r\n";
}

std::string HttpResponse::buildHeaders(std::string headerName, std::string headerValue)
{
    // TODO: validate headerName
    return headerName + ": " + headerValue + "\r\n";
}

std::string HttpResponse::buildBody(std::string input)
{
    return "\r\n" + input;
}

std::string HttpResponse::buildHttpResponse(int status, std::map<std::string, std::string> headers,
                                            std::string body)
{
    std::string res = "";
    res += HttpResponse::buildStatus(status);
    for (auto x : headers) res += HttpResponse::buildHeaders(x.first, x.second);
    res += HttpResponse::buildBody(body);
    BOOST_LOG_TRIVIAL(trace) << "built http response";
    return res;
}

void HttpResponse::setHttpResponse(int errorCode, std::map<std::string,std::string> headers, std::string body)
{
    this -> errorCode = errorCode;
    this -> headers = headers;
    this -> body = body;
}

std::string HttpResponse::buildHttpResponse()
{
    std::string res = "";
    res += HttpResponse::buildStatus(this -> errorCode);
    for (auto x : this -> headers) res += HttpResponse::buildHeaders(x.first, x.second);
    res += HttpResponse::buildBody(this -> body);
    BOOST_LOG_TRIVIAL(trace) << "built http response";
    return res;
}
