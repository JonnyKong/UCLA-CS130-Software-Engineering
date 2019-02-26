//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "http_response.h"
#include "http_request.h"
#include "request_handler_dispatcher.h"
#include "nginx_config.h"

using boost::asio::ip::tcp;
using ::testing::_;
using ::testing::Invoke;

class session
{
public:
    session(boost::asio::io_service& io_service) : socket_(io_service) {};
    tcp::socket& socket();

    void start(HandlerManager* manager, RequestHandlerDispatcher* dispatcher, NginxConfig* config);
private:
    friend class SessionTest;
    FRIEND_TEST(SessionTest, NonHTTPRequestTest);
    FRIEND_TEST(SessionTest, StaticFileRequestTest);
    FRIEND_TEST(SessionTest, ActionRequestTest);

    void writeToSocket(std::string response);

    RequestHandlerDispatcher* sessionDispatcher;
    HandlerManager* sessionManager;
    NginxConfig* sessionConfig;

    void handleRead(const boost::system::error_code& error,
                    size_t bytes_transferred);
    void handleWrite(const boost::system::error_code& error);
    void finishRead(const boost::system::error_code& error, HttpRequest request);
    tcp::socket socket_;
    enum { max_length = 102400 };
    char data_[max_length];
};
