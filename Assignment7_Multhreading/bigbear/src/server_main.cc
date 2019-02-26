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
#include "server.h"
#include "config_parser.h"
#include <boost/log/trivial.hpp>

using boost::asio::ip::tcp;

void termination_handler(int param)
{
    BOOST_LOG_TRIVIAL(fatal) << "Received the termination signal, shutting down the server...";
    exit(0);
}


// TODO: THis code definitely needs to be refactored
int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        // TODO: This will be the new code after dispatcher/HandlerManager implemeented
        NginxConfig config;
        NginxConfigParser parser;
        std::string listenPort;
        if(parser.Parse(argv[1], &config))
        {
            // config will be used to init server parameters and be passed
            // on to relevant handlers by dispatcher
            listenPort = config.getFlatParameters()["listen"];
        }

//        // TODO: replace all of the below with new config parser
//        NginxConfigParser_old parser;

        // Enable Boost Logs
        initLogging();


//        if(parser.Parse(argv[1]))
//        {
//            listenPort = parser.config->parameters[ConfigParameter::LISTEN_PORT];
//        }

        for (int i = 0; i < listenPort.length(); i++)
        {
            if (!(listenPort[i] >= '0' && listenPort[i] <= '9')) throw 11;
        }
        int intListenPort = stoi(listenPort);
        if (intListenPort >= 65536 || intListenPort < 0)
        {
            BOOST_LOG_TRIVIAL(fatal) << "Bad port number!";
            throw 10; // TODO: probably should return a exceptions class......
        }
          
        BOOST_LOG_TRIVIAL(info) << "Listening port:" << listenPort;
        signal(SIGINT, termination_handler);
        BOOST_LOG_TRIVIAL(info) << "Server is running";
        server s(io_service, intListenPort, &config);
        io_service.run();
      }
      catch (std::exception& e)
      {
            BOOST_LOG_TRIVIAL(fatal) << "Exception: " << e.what() << "\n";
      }

      return 0;
}
