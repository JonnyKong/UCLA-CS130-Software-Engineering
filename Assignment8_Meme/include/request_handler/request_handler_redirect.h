/**
 * Request handler for echo.
 */

#ifndef REQUEST_HANDLER_REDIRECT_H
#define REQUEST_HANDLER_REDIRECT_H

#include <iostream>

#include "request_handler.h"

class RequestHandlerRedirect : public RequestHandler {
public:
    explicit RequestHandlerRedirect(const NginxConfig &config) {}    /* To conform with sibling class */
    std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;
};

#endif  // REQUEST_HANDLER_REDIRECT_H
