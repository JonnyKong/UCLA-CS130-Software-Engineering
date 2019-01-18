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

// Get port number from parsed config.
// Return the first outer-most valid one if there are many, and -1 on error.
int get_port_from_config(const NginxConfig* config) {
  // Sanity check
  if (config == nullptr)
    return -1;

  // First traverse statements without child blocks
  for (auto pStatement : config -> statements_) {
    if (pStatement->child_block_.get() == nullptr) {
      if (pStatement->tokens_.size() == 2 && pStatement->tokens_[0] == "port") {
        int ret = atoi(pStatement->tokens_[1].c_str());
        return (ret >= 0 && ret <= 0xffff) ? ret : -1;  // Valid port range
      }
    }
  }
  // Then traverse statements with child blocks
  for (auto pStatement : config -> statements_) {
    if (pStatement->child_block_.get() != nullptr) {
      int ret;
      if ((ret = get_port_from_config(pStatement->child_block_.get())) != -1)
        return ret;
    }
  }
  return -1;  // Ret type should be int to cover -1
}


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
    if ((port = get_port_from_config(&config)) == -1) {
      std::cerr << "Invalid port number in config file" << std::endl;
      return -1;
    }

    boost::asio::io_service io_service;

    server s(io_service, static_cast<short>(port));
    std::cout << "Starting server on port " << port << std::endl; // Debug output

    io_service.run();
  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}