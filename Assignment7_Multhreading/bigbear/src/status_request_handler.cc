#include "status_request_handler.h"
#include "http_response.h"
#include <boost/log/trivial.hpp>
#include <memory>
#include <unordered_map>

StatusRequestHandler::StatusRequestHandler(const NginxConfig& config)
{
    selfConfig = &config;
}

std::unique_ptr<RequestHandler> StatusRequestHandler::create(const NginxConfig& config, const std::string& root_path)
{   
    StatusRequestHandler statusHandler(config);
    return std::make_unique<StatusRequestHandler>(statusHandler);
}

std::unique_ptr<HttpResponse> StatusRequestHandler::HandleRequest(const HttpRequest &request)
{
    int status = 200;
    std::string body;
    body = selfConfig->getFlatParameters()["statusInfo"];

    std::string contentLengthStr = std::to_string(body.length());
    std::map<std::string,std::string> headers { {"Content-Type", "text/plain"},
                                                {"Content-Length", contentLengthStr}};
    HttpResponse res;
    res.setHttpResponse(status, headers, body);
    std::unique_ptr<HttpResponse> resPtr = std::make_unique<HttpResponse>(res);
    return resPtr;
}
