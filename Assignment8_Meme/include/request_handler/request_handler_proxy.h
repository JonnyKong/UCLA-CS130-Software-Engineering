//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef REQUEST_HANDLER_PROXY_H
#define REQUEST_HANDLER_PROXY_H

#include <string>
#include <ctype.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "request_handler.h"
#include "../http/mime_types.h"
#include "../request_handler_dispatcher.h"

class RequestHandlerProxy : public RequestHandler {
 public:
  explicit RequestHandlerProxy(const NginxConfig &config);
  std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;
  std::unique_ptr<reply> handleRequestHelper(std::string host, std::string port, const request &request_);
  std::string requestToString(const request &request_);
  std::unique_ptr<reply> handleError(std::string error_message, const request &request_);
  std::unique_ptr<reply> getReply(std::string headers, std::string content, const request &request_);
  std::unique_ptr<reply> handleRedirect(const std::string& location_header, const request &request_);
  void parsePathAndQuery(const std::string& relative_url, std::string& path, std::string& query);

 private:
  boost::asio::io_service io_service_;
  std::string server_host_;
  std::string server_port_;
};

#endif
