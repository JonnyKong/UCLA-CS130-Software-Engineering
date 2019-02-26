#ifndef ERROR_REQUEST_HANDLER_H
#define ERROR_REQUEST_HANDLER_H

#include "request_handler.h"
#include "config_parser.h"
#include <memory>
#include "http_response.h"

class ErrorRequestHandler : public RequestHandler
{
public:
	static std::unique_ptr<RequestHandler> create(const NginxConfig& config, const std::string& root_path);
    std::unique_ptr<HttpResponse> HandleRequest(const HttpRequest &request) override;

    ErrorRequestHandler() { status = 400; }
};


#endif //ERROR_REQUEST_HANDLER_H
