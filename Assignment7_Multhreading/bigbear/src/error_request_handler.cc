#include "error_request_handler.h"
#include "http_response.h"
#include <boost/log/trivial.hpp>
#include <memory>


std::unique_ptr<RequestHandler> ErrorRequestHandler::create(const NginxConfig& config, const std::string& root_path)
{
    // TODO: set up attributes based on the config file
    ErrorRequestHandler errorHandler;
    return std::make_unique<ErrorRequestHandler>(errorHandler);
}

std::unique_ptr<HttpResponse> ErrorRequestHandler::HandleRequest(const HttpRequest &request)
{
    BOOST_LOG_TRIVIAL(debug) << "Invalid request URI: " << request.requestURI;
    status = 404;
    std::string body = "404 not found\r\n";
    std::string contentLengthStr = std::to_string(body.length());
    std::map<std::string,std::string> headers { {"Content-Type", "text/plain"},
                                                {"Content-Length", contentLengthStr}};
    HttpResponse res;
    res.setHttpResponse(status, headers, body);
    std::unique_ptr<HttpResponse> resPtr = std::make_unique<HttpResponse>(res);
    return resPtr;
}