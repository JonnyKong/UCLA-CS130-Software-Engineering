#include <iostream>
#include <cassert>

#include "request_handler/request_handler_status.h"
#include "request_handler_dispatcher.h"
#include "session.h"

/**
 * handleRequest() - Return reply same as request.
 */
std::unique_ptr<reply> RequestHandlerStatus::handleRequest(const request &request_) noexcept {

    for (auto it = RequestHandlerDispatcher::handler_configs_.begin(); it != RequestHandlerDispatcher::handler_configs_.end(); ++it){
      std::cout << it->first << " " << it->second->tokens_[1]<< std::endl;
    }

    //HTML table contents for request information
    std::string request_info = "";
    for (auto it = session::request_received_.begin(); it != session::request_received_.end(); ++it){
      for(int j = 0; j <  (it->second).size(); j++){
        request_info += "<tr><td>" + it->first +"</td><td>" + std::to_string((it->second)[j]) + "</td></tr>";
      }
    }

    //HTML table contents for handler information
    std::string handler_info = "";
    for (auto it = RequestHandlerDispatcher::handler_configs_.begin(); it != RequestHandlerDispatcher::handler_configs_.end(); ++it){
        handler_info += "<tr><td>" + it->first +"</td><td>" +  it->second->tokens_[1] + "</td></tr>";
    }

    //HTML content in reply
    std::string content = "<html><head>""<title>Server Status</title></head>"
    "<body><h1>Total number of requests</h1><div>"+ std::to_string(session::request_count) +"</div>"
    "<h2>Detail Request Status</h2>"
    "<table>"
    "<tr><th>URL Requested</th><th>Return Code</th></tr>" + request_info + "</table>"
    "<h2>Request Handlers</h2>"
    "<table>"
    "<tr><th>URL Prefix</th><th>Handler</th></tr>" + handler_info +  "</table>"
    "</body>"
    "</html>";

    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    std::cout << "RequestHandlerStatus::handleRequest()" << std::endl;
    reply_->status = reply::ok;    // 200
    reply_->headers.resize(2);
    reply_->content = content;
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/html";
    return reply_;
}

std::string RequestHandlerStatus::requestToString(const request &request_) {

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
