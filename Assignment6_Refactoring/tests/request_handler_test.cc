#include <iostream>

#include "gtest/gtest.h"
#include "request_handler/request_handler_echo.h"
#include "request_handler/request_handler_static.h"
#include "request_handler/request_handler_error.h"
#include "http/request.h"
#include "http/request_parser.h"
#include "config_parser.h"
#include "http/reply.h"
#include "request_handler_dispatcher.h"  // Include typedefs
#include "http/mime_types.h"

#include "session.h"

class RequestHandlerTest : public :: testing::Test{
protected:
  http::server::request req;
  NginxConfigParser config_parser;
  NginxConfig config;
  http::server::request_parser::result_type result;
  http::server::request_parser request_parser;
  std::unique_ptr<http::server::reply> reply_;
  char c;
  std::ifstream f;
  NginxConfig static_handler_config;
  NginxConfig echo_handler_config;
  NginxConfig error_handler_config;
  NginxConfig  handler_config;

};

TEST_F(RequestHandlerTest, ErrorHandlerRequestTest){
  bool success = config_parser.Parse("conf/http.conf", &config);
  for (int i = 0; i < config.statements_.size(); ++i) {
      if ((config.statements_[i])->tokens_[0] == "handler" && (config.statements_[i])->tokens_[1] == "error")
          handler_config =  *(config.statements_[i])->child_block_;
          for (int j = 0; i < handler_config.statements_.size(); ++j) {
              if (handler_config.statements_[j]->tokens_[0] == "location" &&
                      handler_config.statements_[j]->tokens_[1] == "/error") {
                        error_handler_config = handler_config;
              }
          }
  }
  RequestHandlerError request_handler_error(error_handler_config);
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, input, input + strlen(input));
  std::unique_ptr<reply> rep = request_handler_error.handleRequest(req);
  bool success_1 = rep->status == http::server::reply::not_found && rep->content == http::server::reply::stock_reply(reply::not_found)->content;
  bool success_2 = rep->headers[0].name == "Content-Length" &&  rep->headers[0].value == std::to_string(rep->content.size()) &&  rep->headers[1].name == "Content-Type" &&  rep->headers[1].value == "text/html";
  EXPECT_TRUE(success_1);
  EXPECT_TRUE(success_2);
  // EXPECT_TRUE(true);
}

TEST_F(RequestHandlerTest, ErrorHandlerToStringTest){
  bool success = config_parser.Parse("conf/http.conf", &config);
  for (int i = 0; i < config.statements_.size(); ++i) {
      if ((config.statements_[i])->tokens_[0] == "handler" && (config.statements_[i])->tokens_[1] == "error")
          handler_config =  *(config.statements_[i])->child_block_;
          for (int j = 0; i < handler_config.statements_.size(); ++j) {
              if (handler_config.statements_[j]->tokens_[0] == "location" &&
                      handler_config.statements_[j]->tokens_[1] == "/error") {
                        error_handler_config = handler_config;
              }
          }
  }
  RequestHandlerError request_handler_error(error_handler_config);
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, input, input + strlen(input));
  std::string res = request_handler_error.requestToString(req);
  EXPECT_TRUE(res == "GET / HTTP/1.1\r\nHost www.example.com\r\nConnection close\r\n\r\n");
}

TEST_F(RequestHandlerTest, EchoHandlerToStringTest){
  bool success = config_parser.Parse("conf/http.conf", &config);
  for (int i = 0; i < config.statements_.size(); ++i) {
      if ((config.statements_[i])->tokens_[0] == "handler" && (config.statements_[i])->tokens_[1] == "echo")
          handler_config =  *(config.statements_[i])->child_block_;
          for (int j = 0; i < handler_config.statements_.size(); ++j) {
              if (handler_config.statements_[j]->tokens_[0] == "location" &&
                      handler_config.statements_[j]->tokens_[1] == "/echo") {
                        echo_handler_config = handler_config;
              }
          }
  }
  RequestHandlerEcho request_handler_echo(echo_handler_config);
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, input, input + strlen(input));
  std::string res = request_handler_echo.requestToString(req);
  EXPECT_TRUE(res == "GET / HTTP/1.1\r\nHost www.example.com\r\nConnection close\r\n\r\n");
}

TEST_F(RequestHandlerTest, EchoHandlerRequestTest){
  bool success = config_parser.Parse("conf/http.conf", &config);
  for (int i = 0; i < config.statements_.size(); ++i) {
      if ((config.statements_[i])->tokens_[0] == "handler" && (config.statements_[i])->tokens_[1] == "echo")
          handler_config =  *(config.statements_[i])->child_block_;
          for (int j = 0; i < handler_config.statements_.size(); ++j) {
              if (handler_config.statements_[j]->tokens_[0] == "location" &&
                      handler_config.statements_[j]->tokens_[1] == "/echo") {
                        echo_handler_config = handler_config;
              }
          }
  }
  RequestHandlerEcho request_handler_echo(echo_handler_config);
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, input, input + strlen(input));
  reply_ = request_handler_echo.handleRequest(req);
  // EXPECT_TRUE(res == "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n");
  bool success_1 = reply_->status == http::server::reply::ok & reply_->content == request_handler_echo.requestToString(req) && reply_->headers.size() == 2;
  bool success_2 = reply_->headers[0].name == "Content-Length" &&  reply_->headers[0].value == std::to_string(reply_->content.size()) &&  reply_->headers[1].name == "Content-Type" &&  reply_->headers[1].value == "text/plain";
  EXPECT_TRUE(success_1 && success_2);
}


TEST_F(RequestHandlerTest, StaticHandlerRequestTest){
  bool success = config_parser.Parse("conf/http.conf", &config);
  PathUri prefix = "/static";

  for (int i = 0; i < config.statements_.size(); ++i) {
      if ((config.statements_[i])->tokens_[0] == "handler" && (config.statements_[i])->tokens_[1] == "static")
          handler_config =  *(config.statements_[i])->child_block_;
          for (int j = 0; i < handler_config.statements_.size(); ++j) {
              if (handler_config.statements_[j]->tokens_[0] == "location" &&
                      handler_config.statements_[j]->tokens_[1] == "/static") {
                        static_handler_config = handler_config;
              }
          }
  }
  RequestHandlerStatic request_handler_static(static_handler_config, prefix);
  char input[1024] = "GET /static/data/www/data1.dat HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, input, input + strlen(input));
  reply_ = request_handler_static.handleRequest(req);

  f.open("../data/www/data1.dat");

  std::string body;
  while (f.get(c)) body += c;
  f.close();

  bool success_1 = reply_->status == http::server::reply::ok && reply_->content == body && reply_->headers.size() == 2;
  bool success_2 = reply_->headers[0].name == "Content-Length" &&  reply_->headers[0].value == std::to_string(body.size()) &&  reply_->headers[1].name == "Content-Type"  &&  reply_->headers[1].value == "text/plain";
  EXPECT_TRUE(success_1 && success_2);
}



TEST_F(RequestHandlerTest, StaticHandlerRequestNotFoundTest){
  bool success = config_parser.Parse("conf/http.conf", &config);
  PathUri prefix = "/static";
  for (int i = 0; i < config.statements_.size(); ++i) {
      if ((config.statements_[i])->tokens_[0] == "handler" && (config.statements_[i])->tokens_[1] == "static")
          handler_config =  *(config.statements_[i])->child_block_;
          for (int j = 0; i < handler_config.statements_.size(); ++j) {
              if (handler_config.statements_[j]->tokens_[0] == "location" &&
                      handler_config.statements_[j]->tokens_[1] == "/static") {
                        static_handler_config = handler_config;
              }
          }
  }
  RequestHandlerStatic request_handler_static(static_handler_config, prefix);
  char input[1024] = "GET /static/data/www/data1.data HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, input, input + strlen(input));
  reply_ = request_handler_static.handleRequest(req);

  bool success_1 = reply_->status == http::server::reply::not_found &&
  reply_->content ==   "<html>"
    "<head><title>Not Found</title></head>"
    "<body><h1>404 Not Found</h1></body>"
    "</html>" && reply_->headers.size() == 2;
  bool success_2 = reply_->headers[0].name == "Content-Length" &&  reply_->headers[0].value == std::to_string(reply_->content.size()) &&  reply_->headers[1].name == "Content-Type" &&  reply_->headers[1].value == "text/html";
  EXPECT_TRUE(success_1 && success_2);
}
