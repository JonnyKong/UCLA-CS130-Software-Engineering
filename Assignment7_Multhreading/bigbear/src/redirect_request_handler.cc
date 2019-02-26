#include "redirect_request_handler.h"
#include "http_response.h"
#include <boost/log/trivial.hpp>

// This handler is created to test the redirect function of the proxy handler
// The handler always return a 302 HTTP Response with a Location Header
RedirectRequestHandler::RedirectRequestHandler(
        std::unordered_map<std::string, std::string> config_params, const std::string& root_path):
        RequestHandler()
{
    // validURIMap = actionMap;
    redirect_url = string("http://") + config_params["dest"];
    status = 302;
}

void RedirectRequestHandler::setResponse(HttpResponse& response, const HttpRequest& request)
{
    body = "Redirected to " + redirect_url;
    std::string contentLengthStr = std::to_string(body.length());
    std::map<std::string,std::string> headers { {"Content-Length", contentLengthStr},
                                                {"Location", redirect_url}};
    BOOST_LOG_TRIVIAL(info) << "Redirect hanlder has been invoked redirected to: " << redirect_url;

    response.setHttpResponse(status, headers, body);
}

std::unique_ptr<RequestHandler> RedirectRequestHandler::create(const NginxConfig& config, const std::string& root_path)
{
    RedirectRequestHandler redirectHandler(config.getFlatParameters(), root_path);
    return std::make_unique<RedirectRequestHandler>(redirectHandler);
}

std::unique_ptr<HttpResponse> RedirectRequestHandler::HandleRequest(const HttpRequest &request)
{
    HttpResponse response;
    setResponse(response, request);
    return std::make_unique<HttpResponse>(response);
}
