#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <vector>
#include "http_request.h"
#include "config_parser.h"
#include "nginx_config.h"
#include "http_response.h"

using boost::asio::const_buffer;

class RequestHandler
{
public:
	//The following function must be implemented in all handler subclass
    // static RequestHandler* create(const NginxConfig& config, const std::string& root_path);
    RequestHandler() = default;
    virtual std::unique_ptr<HttpResponse> HandleRequest(const HttpRequest &request) = 0;
    virtual ~RequestHandler() = default;
protected:
    int status;
};


#endif // REQUEST_HANDLER_H
