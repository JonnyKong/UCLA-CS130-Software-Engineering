#include <iostream>
#include <cassert>

#include "request_handler/request_handler_health.h"
#include "session.h"

/**
 * handleRequest() - Return reply same as request.
 */
std::unique_ptr<reply> RequestHandlerHealth::handleRequest(const request &request_) noexcept {
    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    std::cout << "RequestHandlerEcho::handleRequest()" << std::endl;
    reply_->status = reply::ok;    // 200
    reply_->headers.resize(2);
    reply_->content = "OK\n";
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/plain";

    //update the request records
    session::request_count++;
    session::request_received_[request_.uri].push_back(reply_->status);
    return reply_;
}
