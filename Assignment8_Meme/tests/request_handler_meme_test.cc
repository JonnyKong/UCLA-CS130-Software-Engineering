#include <iostream>

#include "gtest/gtest.h"
#include "request_handler/request_handler_meme_form.h"
#include "request_handler/request_handler_meme_list.h"
#include "request_handler/request_handler_meme_view.h"
#include "request_handler/request_handler_meme_create.h"
#include "http/request.h"
#include "http/request_parser.h"
#include "config_parser.h"
#include "http/reply.h"
#include "request_handler_dispatcher.h"  // Include typedefs
#include "http/mime_types.h"

#include "session.h"

class RequestHandlerMemeTest : public :: testing::Test{
protected:
  http::server::request req;
  NginxConfigParser config_parser;
  NginxConfig config;
  http::server::request_parser::result_type result;
  http::server::request_parser request_parser;
  std::unique_ptr<http::server::reply> reply_;
  char c;
  NginxConfig handler_config;
  NginxConfig meme_form_handler_config;
  NginxConfig meme_list_handler_config;
};


TEST_F(RequestHandlerMemeTest, MemeFormTest){
  bool success = config_parser.Parse("conf/http.conf", &config);
  for (int i = 0; i < config.statements_.size(); ++i) {
      if ((config.statements_[i])->tokens_[0] == "handler" && (config.statements_[i])->tokens_[1] == "memeForm")
          handler_config =  *(config.statements_[i])->child_block_;
          for (int j = 0; i < handler_config.statements_.size(); ++j) {
              if (handler_config.statements_[j]->tokens_[0] == "location" &&
                      handler_config.statements_[j]->tokens_[1] == "/meme/new") {
                        meme_form_handler_config = handler_config;
              }
          }
  }
  RequestHandlerMemeForm request_handler_meme_form(meme_form_handler_config);
  char input[1024] = "GET /meme/new HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, input, input + strlen(input));

  std::string form = "<form action=\"/meme/create\" method=\"GET\">"
    "<select name=\"image\">"
    "<option>Select a template...</option>"
    "<option value=\"simply.jpg\">one does not simply</option>"
    "<option value=\"grumpy.jpg\">grumpy cat</option>"
    "<option value=\"rubberduck\">rubber duck</option>"
    "</select><br>"
    "<input type=\"text\" name=\"top\" placeholder=\"Top text...\" required><br>"
    "<input type=\"text\" name=\"bottom\" placeholder=\"Bottom text...\" required><br>"
    "<input type=\"submit\" value=\"Create\">"
    "</form>";


  std::unique_ptr<reply> rep = request_handler_meme_form.handleRequest(req);
  bool success_1 = rep->status == http::server::reply::ok && rep->content == form;
  bool success_2 = rep->headers[0].name == "Content-Length" &&  rep->headers[0].value == std::to_string(rep->content.size()) &&  rep->headers[1].name == "Content-Type" &&  rep->headers[1].value == "text/html";
  EXPECT_TRUE(success_1);
  EXPECT_TRUE(success_2);
}

TEST_F(RequestHandlerMemeTest, MemeListLayout){
  bool success = config_parser.Parse("conf/http.conf", &config);
  for (int i = 0; i < config.statements_.size(); ++i) {
      if ((config.statements_[i])->tokens_[0] == "handler" && (config.statements_[i])->tokens_[1] == "memeForm")
          handler_config =  *(config.statements_[i])->child_block_;
          for (int j = 0; i < handler_config.statements_.size(); ++j) {
              if (handler_config.statements_[j]->tokens_[0] == "location" &&
                      handler_config.statements_[j]->tokens_[1] == "/meme/new") {
                        meme_list_handler_config = handler_config;
              }
          }
  }

  RequestHandlerMemeList request_handler_meme_list(meme_list_handler_config);
  char input[1024] = "GET /meme/all HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, input, input + strlen(input));

  std::vector<MemeEntry> meme_list = request_handler_meme_list.selectAllMeme();
  std::string expected_html;
  for(int i = 0; i < meme_list.size(); i++) {
      int id = meme_list[i].id;
      expected_html += boost::str(boost::format("<a href= \"/meme/view?id=%s\">Meme ID: %s</a><br>")% id % id);
  }
  expected_html = "<html><body>" + expected_html + "</body></html>"; 
  std::unique_ptr<reply> rep = request_handler_meme_list.handleRequest(req);
  bool success_1 = rep->status == http::server::reply::ok && rep->content == expected_html;
  bool success_2 = rep->headers[0].name == "Content-Length" &&  rep->headers[0].value == std::to_string(rep->content.size()) &&  rep->headers[1].name == "Content-Type" &&  rep->headers[1].value == "text/html";
  EXPECT_TRUE(success_1);
  EXPECT_TRUE(success_2);
}