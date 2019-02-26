#ifndef REDIRECT_REQUEST_HANDLER_H
#define REDIRECT_REQUEST_HANDLER_H

#include "config_parser.h"
#include "http_request.h"
#include "http_response.h"
#include "request_handler.h"
#include "config_parser.h"
#include <memory>

class RedirectRequestHandler : public RequestHandler
{
public:
	static std::unique_ptr<RequestHandler> create(const NginxConfig& config, const std::string& root_path);
    std::unique_ptr<HttpResponse> HandleRequest(const HttpRequest &request) override;
	void setResponse(HttpResponse& response, const HttpRequest& request);

	RedirectRequestHandler(std::unordered_map<std::string, std::string> actionMap, const std::string& root_path);
    RedirectRequestHandler() = default; // for testing
    void clearVariables();

    std::string redirect_url;
    std::string body;

};


#endif //REDIRECT_REQUEST_HANDLER_H
