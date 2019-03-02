#include "gtest/gtest.h"
#include "logger.h"
#include<iostream>
#include<fstream>
#include "http/request.h"
#include "http/request_parser.h"
#include <boost/asio.hpp>
#include "session.h"




class LoggerTest : public :: testing::Test{
protected:
//   std::string readLogFile(std::string filename){
// 	std::ifstream logfile;
// 	logfile.open(filename);
// 	std::string body;
// 	char c;
// 	while (logfile.get(c)) body += c;
//     logfile.close();
//     int start_idx = body.rfind("]") + 2;
//     std::string real_msg = body.substr(start_idx);

//     return real_msg;
// }

  Logger * logger = Logger::getLogger();
  char c;
  std::ifstream logfile;
  std::string Message = "message";
};

TEST_F(LoggerTest, logServerInitialization){
	std::string expected_msg = "Trace: Server has been initialized\n";
	logger->logServerInitialization();

  	logfile.open("../log/SYSLOG_0.log");

  	std::string body;
    while (logfile.get(c)) body += c;
    logfile.close();
    int start_idx = body.rfind("]") + 2;
    std::string real_msg = body.substr(start_idx);

	// std::string real_msg = readLogFile("../log/SYSLOG_0.log")
    EXPECT_TRUE(real_msg == expected_msg);

}

TEST_F(LoggerTest, TraceFile){
	std::string expected_msg = "Trace: " + Message + "\n";
	logger->logTraceFile(Message);
  	
  	logfile.open("../log/SYSLOG_0.log");

  	std::string body;
    while (logfile.get(c)) body += c;
    logfile.close();
    int start_idx = body.rfind("]") + 2;
    std::string real_msg = body.substr(start_idx);
    EXPECT_TRUE(real_msg == expected_msg);

}

TEST_F(LoggerTest, logErrorFile){
	std::string expected_msg = "Error: " + Message + "\n";
	logger->logErrorFile(Message);
  	
  	logfile.open("../log/SYSLOG_0.log");

  	std::string body;
    while (logfile.get(c)) body += c;
    logfile.close();
    int start_idx = body.rfind("]") + 2;
    std::string real_msg = body.substr(start_idx);
    EXPECT_TRUE(real_msg == expected_msg);

}

TEST_F(LoggerTest, logDebugFile){
	std::string expected_msg = "Debug: " + Message + "\n";
	logger->logDebugFile(Message);
  	
  	
  	logfile.open("../log/SYSLOG_0.log");

  	std::string body;
    while (logfile.get(c)) body += c;
    logfile.close();
    int start_idx = body.rfind("]") + 2;
    std::string real_msg = body.substr(start_idx);
    EXPECT_TRUE(real_msg == expected_msg);

}

TEST_F(LoggerTest, logWarningFile){
	
	std::string expected_msg = "Warning: " + Message + "\n";
	logger->logWarningFile(Message);
  	
  	logfile.open("../log/SYSLOG_0.log");

  	std::string body;
    while (logfile.get(c)) body += c;
    logfile.close();
    int start_idx = body.rfind("]") + 2;
    std::string real_msg = body.substr(start_idx);
    EXPECT_TRUE(real_msg == expected_msg);

}

TEST_F(LoggerTest, logSig){
  
  std::string expected_msg = "Warning: Shutting down the server...\n";
  logger->logSig();
    
    logfile.open("../log/SYSLOG_0.log");

    std::string body;
    while (logfile.get(c)) body += c;
    logfile.close();
    int start_idx = body.rfind("]") + 2;
    std::string real_msg = body.substr(start_idx);
    EXPECT_TRUE(real_msg == expected_msg);

}

// TEST_F(LoggerTest, logTraceHTTPrequest){

//     http::server::request req;
//     http::server::request_parser parser;
//     http::server::request_parser::result_type result;
//     tcp::socket& socket = session::socket();
//     char input[1024] = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
//     std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
//     std::string expected_msg = "Trace: GET www.example.com HTTP 1.1 Sender IP: 127.0.0.1\n";
//     logger->logTraceHTTPrequest(req, socket);
    
//     logfile.open("../log/SYSLOG_0.log");

//     std::string body;
//     while (logfile.get(c)) body += c;
//     logfile.close();
//     int start_idx = body.rfind("]") + 2;
//     std::string real_msg = body.substr(start_idx);
//     EXPECT_TRUE(real_msg == expected_msg);

// }