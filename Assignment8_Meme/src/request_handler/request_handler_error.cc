#include <iostream>
#include <cassert>

#include "request_handler/request_handler_error.h"

/**
 * handleRequest() - Return reply same as request.
 */
std::unique_ptr<reply> RequestHandlerError::handleRequest(const request &request_) noexcept {
    std::cout << "RequestHandlerError::handleRequest()" << std::endl;
    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    reply_ = http::server::reply::stock_reply(reply::not_found);

    return reply_;
}
