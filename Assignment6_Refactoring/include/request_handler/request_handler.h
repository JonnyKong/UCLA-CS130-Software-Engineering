/**
 * Abstract class for request handlers.
 */

#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <iostream>
#include "../config_parser.h"

#include "../http/request_parser.h"
#include "../http/request.h"
#include "../http/reply.h"

using http::server::request;
using http::server::request_parser;
using http::server::reply;

class RequestHandler {
public:
    virtual std::unique_ptr<reply> handleRequest(const request &request_) noexcept = 0;

protected:
    
};

#endif  // REQUEST_HANDLER_H