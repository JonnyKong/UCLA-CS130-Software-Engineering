#ifndef ACTION_REQUEST_HANDLER_H
#define ACTION_REQUEST_HANDLER_H

#include "config_parser.h"
#include "http_request.h"
#include "http_response.h"
#include "request_handler.h"
#include "config_parser.h"
#include <memory>

class ActionRequestHandler : public RequestHandler
{
public:
	static std::unique_ptr<RequestHandler> create(const NginxConfig& config, const std::string& root_path);
    std::unique_ptr<HttpResponse> HandleRequest(const HttpRequest &request) override;
	void setResponse(HttpResponse& response, const HttpRequest& request);

	ActionRequestHandler(std::unordered_map<ServerAction, std::string> serverActionMap);
	ActionRequestHandler(std::unordered_map<std::string, std::string> actionMap, const std::string& root_path);
    ActionRequestHandler() = default; // for testing
    void clearVariables();
    std::string echoRequest();

    std::string URI;
    std::unordered_map<ServerAction, std::string> validActionMap;
    std::unordered_map<std::string, std::string> validURIMap;
    std::string body;

};


#endif //ACTION_REQUEST_HANDLER_H
