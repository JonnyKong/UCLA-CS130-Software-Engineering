/**
 * Request handler for showing meme creation form.
 */

#ifndef REQUEST_HANDLER_MEME_FORM_H
#define REQUEST_HANDLER_MEME_FORM_H

#include <iostream>

#include "request_handler.h"

class RequestHandlerMemeForm : public RequestHandler {
public:
    explicit RequestHandlerMemeForm(const NginxConfig &config) {}    /* To conform with sibling class */
    std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;

    std::string form_;
};

#endif  // REQUEST_HANDLER_MEME_FORM_H
