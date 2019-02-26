#include "gtest/gtest.h"
#include "http/reply.h"
#include "session.h"
#include "gmock/gmock.h"
#include "server.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using ::testing::_;
using ::testing::AtLeast;
class MockSession : public session{
  public:
    boost::asio::io_service m_io_service;
    MockSession() : session(m_io_service){}
    MOCK_METHOD0(handle_read, void());
    MOCK_METHOD0(handle_write, void());
    MOCK_METHOD2(echo_reply, http::server::reply (const char *data_, int bytes_transferred));
};

TEST(ServerTest, BasicServerTest) {
    MockSession m_session;
    EXPECT_CALL(m_session, handle_read()).Times(AtLeast(1));
    short port = 8080;
    server m_server(m_session.m_io_service, port, m_session);
    EXPECT_TRUE(true);
}
