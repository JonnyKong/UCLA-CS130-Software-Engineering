#ifndef STATUS_REQUEST_HANDLER_H
#define STATUS_REQUEST_HANDLER_H

#include "request_handler.h"
#include "config_parser.h"
#include <memory>
#include "http_response.h"

class StatusRequestHandler : public RequestHandler
{
public:

	StatusRequestHandler(const NginxConfig& config);
	~StatusRequestHandler() = default;

    static std::unique_ptr<RequestHandler> create(const NginxConfig& config, const std::string& root_path);
    std::unique_ptr<HttpResponse> HandleRequest(const HttpRequest &request) override;
private:
    const NginxConfig* selfConfig;
    std::string retrieveStatusInfo(const NginxConfig& config);
};


#endif //STATUS_REQUEST_HANDLER_H
