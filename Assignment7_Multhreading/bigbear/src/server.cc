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
#include <thread>
#include <mutex>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include "server.h"
#define BOOST_LOG_DYN_LINK 1
using boost::asio::ip::tcp;
namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;


// this function creates a new session object which in turn initializes a socket
// object. It then calls async_accept which allows server to accept
// data streams over its socket connection after a socket connection is established
bool server::start_accept()
{
    session* new_session = new session(io_service_); // create new session + initialize socket object
    acceptor_.async_accept(new_session->socket(), // wait for a socket connection in a thread
                           boost::bind(&server::handle_accept, this, new_session, // call handle_accept once a socket connection is established
                                       boost::asio::placeholders::error));
    waitingForConnection_ = true;
    return waitingForConnection_;
}

// handle_accept calls new_session->start() if a socket connection is established
bool server::handle_accept(session* new_session,
                           const boost::system::error_code& error)
{
    if (!error)
    {
        BOOST_LOG_TRIVIAL(trace) << "TCP socket connection established";
        std::thread newThread(&server::startThread, this, new_session);
        BOOST_LOG_TRIVIAL(trace) << "New thread handling connection. ID: " << newThread.get_id();
        newThread.detach();
        clientConnectionEstablished_ = true;
    }
    else
    {
        delete new_session;
        clientConnectionEstablished_ = false;
    }
    
    start_accept();
    return clientConnectionEstablished_;
}

void server::startThread(session *new_session)
{
    auto dispatcher = new RequestHandlerDispatcher(*serverConfig);
    new_session->start(manager, dispatcher, serverConfig);
}

// Sets up request handler objects
void server::init(NginxConfig* config)
{
    manager = new HandlerManager;
    serverConfig = config;
}

void initLogging()
{
    logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");
    std::string format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%";
    logging::add_file_log(keywords::file_name="./log/%N.log",
                          // the followin line change logging into append mode
                          // whether this is a good idea may need further discussion
                          // keywords::open_mode=std::ios_base::app, 
                          keywords::auto_flush = true,
                          keywords::rotation_size = 10 * 1024 * 1024,
                          keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
                          keywords::format = format);
    logging::add_console_log(std::clog, keywords::format = format);
    logging::add_common_attributes();
    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::trace // switch to change logging severity filter
    );
}
