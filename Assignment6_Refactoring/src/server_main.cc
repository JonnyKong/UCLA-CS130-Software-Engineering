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
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "session.h"
#include "server.h"
#include "config_parser.h"
#include "logger.h"
#include "request_handler_dispatcher.h"
using boost::asio::ip::tcp;

void my_handler(int s){
    Logger * logger = Logger::getLogger();
    logger->logSig();
    exit(1);
}
int main(int argc, char* argv[])
{
  try
  {
    Logger *logger = Logger::getLogger();
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    if (argc != 2) {
      std::cerr << "Usage: " << argv[0] << " <port>\n";
      logger->logErrorFile("wrong usage port is needed");
      return 1;
    }

    // Parse config from input file
    NginxConfigParser parser;
    NginxConfig config;
    logger->logTraceFile("Parsing the config file...");
    if (!parser.Parse(argv[1], &config)) {
      logger->logErrorFile("Pasing is failed");
      return -1;
    }
    logger->logTraceFile("Done parsing!");
    int port;
    if ((port = config.get_port_from_config()) == -1) {
      logger->logErrorFile("Invalid port number in config file");
      return -1;
    }
    boost::asio::io_service m_io_service;
    // session m_session(m_io_service, nullptr);   // First session not used
    server s(m_io_service, static_cast<short>(port), config);
    logger->logServerInitialization();
    logger->logTraceFile("Starting server on port: " + std::to_string(port));
    m_io_service.run();
  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
