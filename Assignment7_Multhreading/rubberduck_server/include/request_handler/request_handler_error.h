#ifndef REQUEST_HANDLER_ERROR_H
#define REQUEST_HANDLER_ERROR_H

#include <iostream>

#include "request_handler.h"

class RequestHandlerError : public RequestHandler {
public:
    explicit RequestHandlerError(const NginxConfig &config) {}    /* To conform with sibling class */
    std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;

// private:
    std::string requestToString(const request &request_);
};

#endif  // REQUEST_HANDLER_ERROR_H