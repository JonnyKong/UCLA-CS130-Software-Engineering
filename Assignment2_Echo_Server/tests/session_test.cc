/**
 * Unit tests for session request callbacks.
 */

#include "gtest/gtest.h"
#include "session.h"

class SessionTest : public :: testing::Test {
protected:
  boost::asio::io_service io_service;
  std::shared_ptr<session> new_session = std::make_shared<session>(session(io_service));
};

// Good request should return 0
TEST_F(SessionTest, GoodRequest) {
  sprintf(new_session -> data_, "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n");
  int ret = new_session -> handle_read_callback(new_session->shared_from_this(), 
                                                boost::system::error_code(), 
                                                strlen(new_session->data_));
  EXPECT_EQ(ret, 0);
}

// Bad request should return 1
TEST_F(SessionTest, BadRequest) {
  sprintf(new_session -> data_, "GET HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n");
  int ret = new_session -> handle_read_callback(new_session->shared_from_this(), 
                                                boost::system::error_code(), 
                                                strlen(new_session->data_));
  EXPECT_EQ(ret, 1);
}

// Indeterminate request should return 2
TEST_F(SessionTest, IndeterminateRequest) {
  sprintf(new_session -> data_, "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n");
  int ret = new_session -> handle_read_callback(new_session->shared_from_this(), 
                                                boost::system::error_code(), 
                                                strlen(new_session->data_));
  EXPECT_EQ(ret, 2);
}

// Write callback should return 0
TEST_F(SessionTest, WriteCallback) {
  int ret = new_session -> handle_read_callback(new_session->shared_from_this(), 
                                                boost::system::error_code(), 
                                                strlen(new_session->data_));
  EXPECT_EQ(ret, 2);
}

// Echo reply should have 200 status
TEST_F(SessionTest, EchoReply) {
  sprintf(new_session -> data_, "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n");
  reply reply_ = new_session -> echo_reply(new_session -> data_, 
                                           strlen(new_session -> data_));
  EXPECT_EQ(reply_.status, 200);
}
