#include "gtest/gtest.h"
#include "http/request_parser.h"
#include "http/request.h"


class RequestParserTest : public :: testing::Test{
protected:
  http::server::request_parser parser;
  http::server::request req;
  http::server::request_parser::result_type result;
};

TEST_F(RequestParserTest, GoodRequest) {

  //A good HTTP request
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success = result == http::server::request_parser::good;
  EXPECT_TRUE(success);
}

TEST_F(RequestParserTest, BadRequest) {

  //missing a slash in HTTP request, the parser should return bad
  char input[1024] = "GET HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success = result == http::server::request_parser::bad;
  EXPECT_TRUE(success);
}

TEST_F(RequestParserTest, IndeterminateRequest) {

  //Incomplete HTTP request, the parser should return indeterminate
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success = result == http::server::request_parser::indeterminate;
  EXPECT_TRUE(success);
}


TEST_F(RequestParserTest, ParserTest1) {

  //Test whether the parser return correct results to request struct
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success_1 = req.method == "GET" && req.uri == "/" && req.http_version_major == 1 && req.http_version_minor == 1;
  bool success_2 = req.headers[0].name == "Host" && req.headers[0].value == "www.example.com";
  bool success_3 = req.headers[1].name == "Connection" && req.headers[1].value == "close";
  EXPECT_TRUE(success_1 && success_2 && success_3);
}

TEST_F(RequestParserTest, ParserTest2) {

  //Test whether the parser return correct results to request struct, test for version number correctness
  char input[1024] = "POST / HTTP/10.10\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success_1 = req.method == "POST" && req.uri == "/" && req.http_version_major == 10 && req.http_version_minor == 10;
  bool success_2 = req.headers[0].name == "Host" && req.headers[0].value == "www.example.com";
  bool success_3 = req.headers[1].name == "Connection" && req.headers[1].value == "close";
  EXPECT_TRUE(success_1 && success_2 && success_3);
}


TEST_F(RequestParserTest, ParserTest3) {

  //Test for invalid HTTP request parse result
  char input[1024] = "GET HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  // method should be "GET", uri should be "HTTP/1.1", version number should be 0, headers vector should be empty
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success_1 = req.method == "GET" && req.uri == "HTTP/1.1" &&  req.http_version_major == 0 &&  req.http_version_minor == 0;
  bool success_2 = req.headers.empty();
  EXPECT_TRUE(success_1 && success_2 );
}
