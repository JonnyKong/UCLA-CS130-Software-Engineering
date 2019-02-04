/**
 * Request handler for echo.
 */

#ifndef REQUEST_HANDLER_ECHO_H
#define REQUEST_HANDLER_ECHO_H

#include <iostream>

#include "request_handler.h"

class RequestHandlerEcho : public RequestHandler {
public:
    explicit RequestHandlerEcho(const NginxConfig &config) {}    /* To conform with sibling class */
    void handleRequest(const request &request_, reply *reply_) noexcept override;

// private:
    std::string requestToString(const request &request_);
};

#endif  // REQUEST_HANDLER_ECHO_H
