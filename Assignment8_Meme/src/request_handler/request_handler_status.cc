#include <iostream>
#include <cassert>
#include "request_handler/request_handler_status.h"
#include "request_handler_dispatcher.h"

/**
 * handleRequest() - Return reply same as request.
 */
std::unique_ptr<reply> RequestHandlerStatus::handleRequest(const request &request_) noexcept {

    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    std::cout << "RequestHandlerStatus::handleRequest()" << std::endl;
    reply_->status = reply::ok;    // 200

    std::string page_style = "<style>table{width = \"500\";}table,th,td{border:1px solid black;}td{width = \"400\";}</style>";

    //HTML table contents for request information
    for (auto it = session::request_received_.begin(); it != session::request_received_.end(); ++it){
      for(int j = 0; j <  (it->second).size(); j++){
        request_info += "<tr><td>" + it->first +"</td><td>" + std::to_string((it->second)[j]) + "</td></tr>";
      }
    }

    //HTML table contents for handler information
    for (auto it = RequestHandlerDispatcher::handler_configs_.begin(); it != RequestHandlerDispatcher::handler_configs_.end(); ++it){
        handler_info += "<tr><td>" + it->first +"</td><td>" +  it->second->tokens_[1] + "</td></tr>";
    }

    //HTML content in reply

    std::string display_html_content = "<html><head>" + page_style +
    "<title>Server Status</title></head>\n"
    "<body><h2>Total number of requests</h1><div>"+ std::to_string(session::request_count) +"</div>\n"
    "<h2>Detail Request Status</h2>\n"
    "<table>\n"
    "<tr><th>URL Requested</th><th>Return Code</th></tr>" + request_info + "</table>\n"
    "<h2>Request Handlers</h2>\n"
    "<table>\n"
    "<tr><th>URL Prefix</th><th>Handler</th></tr>" + handler_info +  "</table>\n"
    "</body>\n"
    "</html>\n";


    reply_->headers.resize(2);
    reply_->content = display_html_content;
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/html";

    return reply_;
}
