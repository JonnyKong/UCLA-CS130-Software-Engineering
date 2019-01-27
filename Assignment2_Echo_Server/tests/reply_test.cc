#include "gtest/gtest.h"
#include "http/reply.h"
//
//
class ReplyTest : public :: testing::Test{
protected:
  http::server::reply reply_;
  std::vector<boost::asio::const_buffer> buffers;
};


TEST_F(ReplyTest, Ok) {
  reply_ = http::server::reply::stock_reply(http::server::reply::ok);
  bool success_1 = reply_.status == http::server::reply::ok && reply_.content == "" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}


TEST_F(ReplyTest, Created) {
  reply_ = http::server::reply::stock_reply(http::server::reply::created);
  bool success_1 = reply_.status == http::server::reply::created && reply_.content ==   "<html>"
    "<head><title>Created</title></head>"
    "<body><h1>201 Created</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, accepted) {
  reply_ = http::server::reply::stock_reply(http::server::reply::accepted);
  bool success_1 = reply_.status == http::server::reply::accepted && reply_.content ==   "<html>"
    "<head><title>Accepted</title></head>"
    "<body><h1>202 Accepted</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, no_content) {
  reply_ = http::server::reply::stock_reply(http::server::reply::no_content);
  bool success_1 = reply_.status == http::server::reply::no_content &&
  reply_.content ==  "<html>"
  "<head><title>No Content</title></head>"
  "<body><h1>204 Content</h1></body>"
  "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, multiple_choices) {
  reply_ = http::server::reply::stock_reply(http::server::reply::multiple_choices);
  bool success_1 = reply_.status == http::server::reply::multiple_choices &&
  reply_.content ==   "<html>"
    "<head><title>Multiple Choices</title></head>"
    "<body><h1>300 Multiple Choices</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, moved_permanently) {
  reply_ = http::server::reply::stock_reply(http::server::reply::moved_permanently);
  bool success_1 = reply_.status == http::server::reply::moved_permanently &&
  reply_.content ==    "<html>"
    "<head><title>Moved Permanently</title></head>"
    "<body><h1>301 Moved Permanently</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, moved_temporarily) {
  reply_ = http::server::reply::stock_reply(http::server::reply::moved_temporarily);
  bool success_1 = reply_.status == http::server::reply::moved_temporarily &&
  reply_.content ==    "<html>"
    "<head><title>Moved Temporarily</title></head>"
    "<body><h1>302 Moved Temporarily</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, not_modified) {
  reply_ = http::server::reply::stock_reply(http::server::reply::not_modified);
  bool success_1 = reply_.status == http::server::reply::not_modified &&
  reply_.content ==    "<html>"
    "<head><title>Not Modified</title></head>"
    "<body><h1>304 Not Modified</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, bad_request) {
  reply_ = http::server::reply::stock_reply(http::server::reply::bad_request);
  bool success_1 = reply_.status == http::server::reply::bad_request &&
  reply_.content ==     "<html>"
    "<head><title>Bad Request</title></head>"
    "<body><h1>400 Bad Request</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, unauthorized) {
  reply_ = http::server::reply::stock_reply(http::server::reply::unauthorized);
  bool success_1 = reply_.status == http::server::reply::unauthorized &&
  reply_.content ==    "<html>"
    "<head><title>Unauthorized</title></head>"
    "<body><h1>401 Unauthorized</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, forbidden) {
  reply_ = http::server::reply::stock_reply(http::server::reply::forbidden);
  bool success_1 = reply_.status == http::server::reply::forbidden &&
  reply_.content ==    "<html>"
    "<head><title>Forbidden</title></head>"
    "<body><h1>403 Forbidden</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}


TEST_F(ReplyTest, not_found) {
  reply_ = http::server::reply::stock_reply(http::server::reply::not_found);
  bool success_1 = reply_.status == http::server::reply::not_found &&
  reply_.content ==   "<html>"
    "<head><title>Not Found</title></head>"
    "<body><h1>404 Not Found</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, internal_server_error) {
  reply_ = http::server::reply::stock_reply(http::server::reply::internal_server_error);
  bool success_1 = reply_.status == http::server::reply::internal_server_error &&
  reply_.content ==     "<html>"
    "<head><title>Internal Server Error</title></head>"
    "<body><h1>500 Internal Server Error</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, not_implemented) {
  reply_ = http::server::reply::stock_reply(http::server::reply::not_implemented);
  bool success_1 = reply_.status == http::server::reply::not_implemented &&
  reply_.content ==     "<html>"
    "<head><title>Not Implemented</title></head>"
    "<body><h1>501 Not Implemented</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}


TEST_F(ReplyTest, bad_gateway) {
  reply_ = http::server::reply::stock_reply(http::server::reply::bad_gateway);
  bool success_1 = reply_.status == http::server::reply::bad_gateway &&
  reply_.content ==     "<html>"
    "<head><title>Bad Gateway</title></head>"
    "<body><h1>502 Bad Gateway</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, service_unavailable) {
  reply_ = http::server::reply::stock_reply(http::server::reply::service_unavailable);
  bool success_1 = reply_.status == http::server::reply::service_unavailable &&
  reply_.content ==    "<html>"
    "<head><title>Service Unavailable</title></head>"
    "<body><h1>503 Service Unavailable</h1></body>"
    "</html>" && reply_.headers.size() == 2;
  bool success_2 = reply_.headers[0].name == "Content-Length" &&  reply_.headers[0].value == std::to_string(reply_.content.size()) &&  reply_.headers[1].name == "Content-Type" &&  reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}
