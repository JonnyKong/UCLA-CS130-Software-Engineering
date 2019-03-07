/**
 * Request handler for showing meme creation form.
 */

#ifndef REQUEST_HANDLER_MEME_FORM_H
#define REQUEST_HANDLER_MEME_FORM_H

#include <iostream>

#include "request_handler.h"
#include "request_handler/meme_common.h"

class RequestHandlerMemeForm : public RequestHandler {
protected:
    /* For unit testing mock class delegate constructor */
    explicit RequestHandlerMemeForm() {}
public:
    explicit RequestHandlerMemeForm(const NginxConfig &config);    /* To conform with sibling class */
    std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;

    std::string form_;
    MemeEntry fetchMemeEntry(const std::string &id);
    bool is_number(const std::string& s);

    std::string database_name;
};

#endif  // REQUEST_HANDLER_MEME_FORM_H
