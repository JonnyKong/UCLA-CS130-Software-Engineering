#include <iostream>

#include "gtest/gtest.h"
#include "request_handler/request_handler_echo.h"
#include "request_handler/request_handler_static.h"
#include "http/request.h"
#include "http/request_parser.h"
#include "config_parser.h"
#include "http/reply.h"
#include "request_handler_dispatcher.h"  // Include typedefs
#include "http/mime_types.h"


class RequestHandlerTest : public :: testing::Test{
protected:
  http::server::request req;
  NginxConfigParser config_parser;
  NginxConfig config;
  http::server::request_parser::result_type result;
  http::server::request_parser request_parser;
  http::server::reply reply_;
  char c;
  std::ifstream f;


};

TEST_F(RequestHandlerTest, EchoHandlerToStringTest){
  bool success = config_parser.Parse("example_config", &config);
  RequestHandlerEcho request_handler_echo(config);
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, input, input + strlen(input));
  std::string res = request_handler_echo.requestToString(req);
  EXPECT_TRUE(res == "GET / HTTP/1.1\r\nHost www.example.com\r\nConnection close\r\n\r\n");
}

TEST_F(RequestHandlerTest, EchoHandlerRequestTest){
  bool success = config_parser.Parse("example_config", &config);
  RequestHandlerEcho request_handler_echo(config);
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, input, input + strlen(input));
  request_handler_echo.handleRequest(req, &reply_);
  // EXPECT_TRUE(res == "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n");
  bool success_1 = reply_.status == http::server::reply::ok & reply_.content == request_handler_echo.requestToString(req) && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/plain";
  EXPECT_TRUE(success_1 && success_2);
}


TEST_F(RequestHandlerTest, StaticHandlerRequestTest){
  bool success = config_parser.Parse("example_config", &config);
  PathUri prefix = "/static";
  RequestHandlerStatic request_handler_static(config, prefix);
  char input[1024] = "GET /static/data/www/data1.dat HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, input, input + strlen(input));
  request_handler_static.handleRequest(req, &reply_);

  f.open("../data/www/data1.dat");

  std::string body;
  while (f.get(c)) body += c;
  f.close();

  bool success_1 = reply_.status == http::server::reply::ok && reply_.content == body && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(body.size()) &&  reply_.headers[1].name == "Content-Type"  &&  reply_.headers[1].value == "text/plain";
  EXPECT_TRUE(success_1 && success_2);
}



TEST_F(RequestHandlerTest, StaticHandlerRequestNotFoundTest){
  bool success = config_parser.Parse("example_config", &config);
  PathUri prefix = "/static";
  RequestHandlerStatic request_handler_static(config, prefix);
  char input[1024] = "GET /static/data/www/data1.data HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, input, input + strlen(input));
  request_handler_static.handleRequest(req, &reply_);

  bool success_1 = reply_.status == http::server::reply::not_found &&
  reply_.content ==   "<html>"
    "<head><title>Not Found</title></head>"
    "<body><h1>404 Not Found</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  EXPECT_TRUE(success_1 && success_2);
}
