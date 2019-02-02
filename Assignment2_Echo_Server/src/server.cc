#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "server.h"
#include "session.h"
#include "logger.h"
using boost::asio::ip::tcp;

server::server(boost::asio::io_service& io_service, short port, 
               const NginxConfig &config)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      dispatcher_(std::make_shared<RequestHandlerDispatcher>(config)) {
    auto m_session = std::make_shared<session>(io_service_, dispatcher_);
    start_accept(*m_session);
}


void server::start_accept(session& m_session) {
    auto new_session = std::make_shared<session>(io_service_, dispatcher_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
            boost::asio::placeholders::error));
}

void server::handle_accept(std::shared_ptr<session> new_session, 
                           const boost::system::error_code& error) {

    if (!error) 
        new_session->start();
    
    start_accept(*new_session);
}