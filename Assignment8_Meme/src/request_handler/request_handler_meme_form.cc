#include <iostream>
#include <cassert>

#include "request_handler/request_handler_meme_form.h"
#include "session.h"

/**
 * handleRequest() - Return an meme creation form.
 */
std::unique_ptr<reply> RequestHandlerMemeForm::handleRequest(const request &request_) noexcept {
  std::unique_ptr<reply> reply_ = std::make_unique<reply>();
  std::cout << "RequestHandlerMemeForm::handleRequest()" << std::endl;
  reply_->status = reply::ok;    // 200

  //update the request records

  session::request_count++;
  session::request_received_[request_.uri].push_back(reply_->status);


  form_ = "<form action=\"/meme/create\" method=\"GET\">"
    "<select name=\"image\">"
    "<option>Select a template...</option>"
    "<option value=\"simply.jpg\">one does not simply</option>"
    "<option value=\"grumpy.jpg\">grumpy cat</option>"
    "</select><br>"
    "<input type=\"text\" name=\"top\" placeholder=\"Top text...\"><br>"
    "<input type=\"text\" name=\"bottom\" placeholder=\"Bottom text...\"><br>"
    "<input type=\"submit\" value=\"Create\">"
    "</form>";


  reply_->headers.resize(2);
  reply_->content = form_;
  reply_->headers[0].name = "Content-Length";
  reply_->headers[0].value = std::to_string((reply_->content).length());
  reply_->headers[1].name = "Content-Type";
  reply_->headers[1].value = "text/html";

  return reply_;
}