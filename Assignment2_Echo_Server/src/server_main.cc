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

#include "session.h"
#include "server.h"
#include "config_parser.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2) {
      std::cerr << "Usage: " << argv[0] << " <port>\n";
      return 1;
    }

    // Parse config from input file
    NginxConfigParser parser;
    NginxConfig config;
    parser.Parse(argv[1], &config);
    int port;
    if ((port = config.get_port_from_config(&config)) == -1) {
      std::cerr << "Invalid port number in config file" << std::endl;
      return -1;
    }
    boost::asio::io_service m_io_service;
    session m_session(m_io_service);
    server s(m_io_service, static_cast<short>(port), m_session);
    std::cout << "Starting server on port " << port << std::endl; // Debug output
    m_io_service.run();
  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
