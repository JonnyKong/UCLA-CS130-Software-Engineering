#include "action_request_handler.h"
#include "http_response.h"
#include <boost/log/trivial.hpp>

ActionRequestHandler::ActionRequestHandler(
        std::unordered_map<ServerAction, std::string> serverActionMap) :
        RequestHandler()
{
    validActionMap = serverActionMap;
    status = 200;
}

ActionRequestHandler::ActionRequestHandler(
        std::unordered_map<std::string, std::string> actionMap, const std::string& root_path):
        RequestHandler()
{
    validURIMap = actionMap;
    status = 200;
}


void ActionRequestHandler::clearVariables()
{
    URI = "";
    status = 200;
}

std::string ActionRequestHandler::echoRequest()
{
    // should it return the raw request back or an http response containing the request in the body?
    // I assume we do the latter
    std::string contentLengthStr = std::to_string(body.length());
    std::map<std::string,std::string> headers { {"Content-Type", "text/plain"},
                                                {"Content-Length", contentLengthStr}};

    HttpResponse response;
    return response.buildHttpResponse(status, headers, body);
}

void ActionRequestHandler::setResponse(HttpResponse& response, const HttpRequest& request)
{
    // should it return the raw request back or an http response containing the request in the body?
    // I assume we do the latter
    std::string contentLengthStr = std::to_string(request.unparsedRequestString.length());
    std::map<std::string,std::string> headers { {"Content-Type", "text/plain"},
                                                {"Content-Length", contentLengthStr}};

    response.setHttpResponse(status, headers, request.unparsedRequestString);
}

std::unique_ptr<RequestHandler> ActionRequestHandler::create(const NginxConfig& config, const std::string& root_path)
{
    ActionRequestHandler actionHandler(config.getFlatParameters(), root_path);
    return std::make_unique<ActionRequestHandler>(actionHandler);
}

std::unique_ptr<HttpResponse> ActionRequestHandler::HandleRequest(const HttpRequest &request)
{
    HttpResponse response;
    setResponse(response, request);
    return std::make_unique<HttpResponse>(response);
}

