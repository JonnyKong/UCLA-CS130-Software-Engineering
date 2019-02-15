#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "session.h"
#include "http/request_parser.h"
#include "http/reply.h"
#include "logger.h"

using boost::asio::ip::tcp;
session::session(boost::asio::io_service& io_service,
                 std::shared_ptr<const RequestHandlerDispatcher> dispatcher)
    : socket_(io_service), dispatcher_(dispatcher) {}

tcp::socket& session::socket() {
    return socket_;
}

void session::start() {
    handle_read();
}

void session::handle_read() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_),
        boost::bind(&session::handle_read_callback, this, self, _1, _2));
    Logger * logger = Logger::getLogger();
    logger->logDebugFile("Read handler");
}

void session::handle_write() {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, reply_->to_buffers(),
        boost::bind(&session::handle_write_callback, this, self, _1, _2));
    Logger * logger = Logger::getLogger();
    logger->logDebugFile("write handler");
}

int session::handle_read_callback(std::shared_ptr<session> self,
                                   boost::system::error_code error,
                                   std::size_t bytes_transferred) {
    Logger * logger = Logger::getLogger();
    if (!error) {
        request_parser::result_type result;
        std::tie(result, std::ignore) = request_parser_.parse(
            request_, data_, data_ + bytes_transferred);
        try { logger->logTraceHTTPrequest(request_, socket_); } catch(...) {}
        if (result == request_parser::good) {
            logger->logDebugFile("Good Request");
            auto handler = dispatcher_ -> getRequestHandler(request_);
            reply_ = handler -> handleRequest(request_);
            session::request_count++;
            session::request_received_[request_.uri].push_back(reply_->status);
            logger->logDebugFile("Reply with status: " + std::to_string(reply_->status));
            handle_write();
            return 0;
        } else if (result == request_parser::bad) {
            logger->logWarningFile("Bad Request");
            reply_ = reply::stock_reply(reply::bad_request);    // Bad request
            session::request_count++;
            session::request_received_[request_.uri].push_back(reply_->status);
            handle_write();
            return 1;
        } else {
            handle_read();
            return 2;
        }
    }
}

int session::handle_write_callback(std::shared_ptr<session> self,
                                    boost::system::error_code error,
                                    std::size_t) {
    if (!error) {
        // Initiate graceful connection closure.
        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                            ignored_ec);
        return 1;
    }
    return 0;
}


int session::request_count = 0;
std::map<URL_Requested, std::vector<ReturnCode>> session::request_received_;
