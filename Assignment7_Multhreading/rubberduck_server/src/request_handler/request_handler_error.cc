#include <iostream>
#include <cassert>

#include "request_handler/request_handler_error.h"
#include "session.h"

/**
 * handleRequest() - Return reply same as request.
 */
std::unique_ptr<reply> RequestHandlerError::handleRequest(const request &request_) noexcept {
    std::cout << "RequestHandlerError::handleRequest()" << std::endl;
    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    reply_ = http::server::reply::stock_reply(reply::not_found);

    //update the request records
    session::request_count++;
    session::request_received_[request_.uri].push_back(reply_->status);

    return reply_;
}

/**
 * requestToString() - Convert request struct to std::string.
 *
 * Because only RequestHandlerEcho uses this function, it is defined here
 *  instead of an member function of request struct.
 */
std::string RequestHandlerError::requestToString(const request &request_) {

    std::string request_str;
    request_str.append(request_.method);
    request_str.append(" ");
    request_str.append(request_.uri);
    request_str.append(" ");

    switch (request_.http_version_minor) {
        case 0:
            request_str.append("HTTP/1.0"); break;
        case 1:
            request_str.append("HTTP/1.1"); break;
        default:
            assert(0);
    }
    request_str.append("\r\n");

    for (http::server::header h : request_.headers) {
        request_str.append(h.name);
        request_str.append(" ");
        request_str.append(h.value);
        request_str.append("\r\n");
    }
    request_str.append("\r\n");

    return request_str;
}
