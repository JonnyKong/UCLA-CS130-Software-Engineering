#ifndef SESSION_H
#define SESSION_H

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "http/request.h"
#include "http/request_parser.h"
#include "http/reply.h"
#include "http/header.h"
#include "request_handler_dispatcher.h"

using boost::asio::ip::tcp;
using http::server::request;
using http::server::request_parser;
using http::server::reply; 

class session : public std::enable_shared_from_this<session>
{
public:
    session(boost::asio::io_service& io_service, 
            std::shared_ptr<const RequestHandlerDispatcher> dispatcher);
    session(std::shared_ptr<const RequestHandlerDispatcher> dispatcher);    // For testing
    tcp::socket& socket();
    void start();

// protected:
    /// Async request handler
    void handle_read();
    void handle_write();

    /// Callbacks that actually handles the requests
    int handle_read_callback(std::shared_ptr<session> self,
                              boost::system::error_code error, 
                              std::size_t bytes_transferred);
    int handle_write_callback(std::shared_ptr<session> self,
                               boost::system::error_code error, 
                               std::size_t);

    tcp::socket socket_;
    enum { max_length = 10240 };
    char data_[max_length];

    /// Send echo reply by reading from request buffer
    // reply echo_reply(const char *data_, int bytes_transferred);

    /// The incoming request.
    request request_;
    /// The parser for the incoming request.
    request_parser request_parser_;
    /// The reply to be sent back to the client.
    std::unique_ptr<reply> reply_;

    /// Dispatcher shouldn't change during a session
    const std::shared_ptr<const RequestHandlerDispatcher> dispatcher_;

    /// For unit testing
    friend class SessionTest;
};

#endif  // SESSION_H