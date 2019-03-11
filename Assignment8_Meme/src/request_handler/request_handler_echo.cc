#include <iostream>
#include <cassert>

#include "request_handler/request_handler_echo.h"
/**
 * handleRequest() - Return reply same as request.
 */
std::unique_ptr<reply> RequestHandlerEcho::handleRequest(const request &request_) noexcept {
    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    std::cout << "RequestHandlerEcho::handleRequest()" << std::endl;
    reply_->status = reply::ok;    // 200
    reply_->headers.resize(2);
    reply_->content = requestToString(request_);
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/plain";

    return reply_;
}

/**
 * requestToString() - Convert request struct to std::string.
 *
 * Because only RequestHandlerEcho uses this function, it is defined here
 *  instead of an member function of request struct.
 */
std::string RequestHandlerEcho::requestToString(const request &request_) {

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
