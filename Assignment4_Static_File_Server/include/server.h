#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "session.h"
#include "request_handler_dispatcher.h"

using boost::asio::ip::tcp;

class session;  // Forward decl to break circular dep

class server {
public:
    server(boost::asio::io_service& io_service, short port, 
           const NginxConfig &config);

    virtual void start_accept(session& m_session);
    virtual void handle_accept(std::shared_ptr<session> new_session, 
                       const boost::system::error_code& error);
                       
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;

private:
    std::shared_ptr<RequestHandlerDispatcher> dispatcher_;
};
#endif  // SERVER_H