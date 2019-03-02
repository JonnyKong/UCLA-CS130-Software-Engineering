#include <iostream>
#include <cassert>

#include "request_handler/request_handler_redirect.h"
#include "session.h"

/**
 * handleRequest() - Return reply same as request.
 */
std::unique_ptr<reply> RequestHandlerRedirect::handleRequest(const request &request_) noexcept {
    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    std::cout << "RequestHandlerRedirect::handleRequest()" << std::endl;
    reply_->status = reply::moved_temporarily;    // 302
    reply_->headers.resize(2);
    reply_->content = "please visit 144.34.150.187";
    reply_->headers[0].name = "Location";
    reply_->headers[0].value = "http://144.34.150.187:3000";
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/plain";

    //update the request records
    session::request_count++;
    session::request_received_[request_.uri].push_back(reply_->status);
    return reply_;
}