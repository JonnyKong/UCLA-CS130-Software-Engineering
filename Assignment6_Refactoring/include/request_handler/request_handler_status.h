/**
 * Request handler for server status.
 */

#ifndef REQUEST_HANDLER_STATUS_H
#define REQUEST_HANDLER_STATUS_H

#include <iostream>

#include "request_handler.h"

class RequestHandlerStatus : public RequestHandler {
public:
    explicit RequestHandlerStatus(const NginxConfig &config):request_info(""),handler_info(""),display_html_content("") {}    /* To conform with sibling class */
    std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;

    std::string requestToString(const request &request_);

private:
    std::string request_info;
    std::string handler_info;
    std::string display_html_content;

};

#endif  // REQUEST_HANDLER_STATUS_H
