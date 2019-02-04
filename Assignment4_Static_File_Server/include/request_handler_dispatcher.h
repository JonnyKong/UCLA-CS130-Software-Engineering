/**
 * A factory-pattern style request handler dispatcher.
 */

#ifndef REQUEST_HANDLER_DISPATCHER_H
#define REQUEST_HANDLER_DISPATCHER_H

#include <iostream> 
#include <map>

#include "config_parser.h"
#include "request_handler/request_handler.h"

typedef std::string PathUri;
// using PathUri = std::string;
typedef const std::string HandlerType;


/* Handler types */
static const HandlerType StaticHandler =    "StaticHandler";
static const HandlerType EchoHandler =      "EchoHandler";


class RequestHandlerDispatcher {
public:
    RequestHandlerDispatcher(const NginxConfig &config);

    virtual std::shared_ptr<RequestHandler> getRequestHandler(const request &request_) const;

private:
    std::map<PathUri, std::shared_ptr<RequestHandler>> handlers_;

    size_t initRequestHandlers(const NginxConfig &config);
    bool registerPath(PathUri path_uri, HandlerType handler_type, const NginxConfig &config);

};

#endif  // REQUEST_HANDLER_DISPATCHER_H