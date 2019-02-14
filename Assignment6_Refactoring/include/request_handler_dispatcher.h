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
typedef std::string HandlerType;


/* Handler types */
static HandlerType StaticHandler =    "static";
static HandlerType EchoHandler =      "echo";


class RequestHandlerDispatcher {
public:
    RequestHandlerDispatcher(const NginxConfig &config);

    virtual std::unique_ptr<RequestHandler> getRequestHandler(const request &request_) const;

private:
    std::map<PathUri, std::shared_ptr<const NginxConfigStatement>> handler_configs_;

    size_t initRequestHandlers(const NginxConfig &config);
    bool registerPath(HandlerType handler_type, std::shared_ptr<const NginxConfigStatement> statement);

};

#endif  // REQUEST_HANDLER_DISPATCHER_H
