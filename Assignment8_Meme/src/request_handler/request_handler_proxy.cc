//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "request_handler/request_handler_proxy.h"

RequestHandlerProxy::RequestHandlerProxy(const NginxConfig &config) : io_service_() {
  for (int i = 0; i < config.statements_.size(); ++i) {
    if (config.statements_[i]->tokens_[0] == "root" && config.statements_[i]->tokens_.size() == 2) {
      server_host_ = config.statements_[i]->tokens_[1];
    } else if (config.statements_[i]->tokens_[0] == "port" && config.statements_[i]->tokens_.size() == 2) {
      server_port_ = config.statements_[i]->tokens_[1];
    }
  }
  BOOST_LOG_TRIVIAL(trace) << "server_host_: " << server_host_;
  BOOST_LOG_TRIVIAL(trace) << "server_port_: " << server_port_;
}

std::unique_ptr<reply> RequestHandlerProxy::handleRequest(const request &request_) noexcept {
  request request_cur;
  request_cur.method = request_.method;
  request_cur.http_version_major = request_.http_version_major;
  request_cur.http_version_minor = request_.http_version_minor;
  request_cur.headers.assign(request_.headers.begin(), request_.headers.end());
  bool flag_empty = true;
  for (int i = 1; i < request_.uri.length(); i++) {
    if (request_.uri.at(i) == '/') {
      request_cur.uri = request_.uri.substr(i, request_.uri.length());
      flag_empty = false;
      break;
    }
  }
  if (flag_empty)
    request_cur.uri = "/";
  return handleRequestHelper(server_host_, server_port_, request_cur);
}

std::unique_ptr<reply> RequestHandlerProxy::handleRequestHelper(std::string host,
                                                                std::string port,
                                                                const request &request_) {
  // stringify the request
  std::string raw_request = requestToString(request_);
  BOOST_LOG_TRIVIAL(trace) << raw_request;

  // resolve the server endpoint
  boost::system::error_code ec;
  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve({host, port}, ec);
  if (ec) return handleError("ERROR encountered when resolving server endpoint", request_);

  // connect the remote server
  boost::asio::ip::tcp::socket socket_(io_service_);
  boost::asio::connect(socket_, endpoint_iterator, ec);
  if (ec) return handleError("ERROR encountered when connecting the remote server", request_);

  // send the request
  boost::asio::streambuf request_stream_buf;
  std::ostream request_stream(&request_stream_buf);
  request_stream << raw_request;
  boost::asio::write(socket_, request_stream_buf, ec);
  if (ec) return handleError("ERROR encountered when sending request", request_);

  // read the response status line. The response streambuf will automatically
  // grow to accommodate the entire line. The growth may be limited by passing
  // a maximum size to the streambuf cFonstructor
  boost::asio::streambuf response_stream_buf;
  boost::asio::read_until(socket_, response_stream_buf, "\r\n", ec);
  if (ec) return handleError("ERROR encountered when reading the response status line", request_);

  // check that the response is OK
  std::string raw_headers = "";
  std::istream response_stream(&response_stream_buf);
  std::string http_version;
  response_stream >> http_version;
  unsigned int status_code;
  response_stream >> status_code;
  std::string status_message;
  std::getline(response_stream, status_message); // contains a space in front of it!
  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    return handleError("ERROR: invalid response", request_);
  raw_headers += http_version + " " + std::to_string(status_code) + status_message + "\r\n";

  // read the response headers, which are terminated by a blank line
  boost::asio::read_until(socket_, response_stream_buf, "\r\n\r\n", ec);
  if (ec) return handleError("ERROR encountered when reading the response headers", request_);

  // process the 302 code
  std::string header;
  if (status_code == 302) {
    BOOST_LOG_TRIVIAL(trace) << "Handling 302 response status code";
    while (std::getline(response_stream, header) && header != "\r") {
      if (header.find("Location") != std::string::npos) {
        header.pop_back();
        return handleRedirect(header, request_);
      }
    }
    return handleError("ERROR encountered when processing 302 code", request_);
  }

  // process the headers
  std::string content_length_str = "";
  while (std::getline(response_stream, header) && header != "\r") {
    if (header.find("Content-Length") != std::string::npos) {
      for (auto c: header) {
        if (isdigit(c))
          content_length_str += c;
      }
    }
    raw_headers += header + "\r\n";
  }

  // check content length in the header
  if (content_length_str == "")
    content_length_str = "0";
  BOOST_LOG_TRIVIAL(trace) << raw_headers;

  // some contents may have been received by read_until
  // according the read_until doc, it may read pass the delimeter
  std::size_t received_content_length = response_stream_buf.size();
  std::size_t left_content_length = boost::lexical_cast<std::size_t>(content_length_str) - received_content_length;
  boost::asio::read(socket_, response_stream_buf, boost::asio::transfer_exactly(left_content_length), ec);
  if (ec) return handleError("ERROR encountered when getting left_content_length", request_);

  // return a reply
  std::string body;
  std::ostringstream ss;
  ss << &response_stream_buf;
  body = ss.str();
  return getReply(raw_headers, body, request_);
}

std::string RequestHandlerProxy::requestToString(const request &request_) {
  std::string request_str;
  request_str.append(request_.method);
  request_str.append(" ");
  request_str.append(request_.uri);
  request_str.append(" ");

  switch (request_.http_version_minor) {
    case 0: request_str.append("HTTP/1.0"); break;
    case 1: request_str.append("HTTP/1.1"); break;
    default: assert(0);
  }
  request_str.append("\r\n");

  for (http::server::header h : request_.headers) {
    request_str.append(h.name);
    request_str.append(": ");
    request_str.append(h.value);
    request_str.append("\r\n");
  }
  request_str.append("\r\n");
  return request_str;
}

std::unique_ptr<reply> RequestHandlerProxy::handleError(std::string error_message,
                                                        const request &request_) {
  BOOST_LOG_TRIVIAL(error) << error_message << "\n";
  std::unique_ptr<reply> reply_ = std::make_unique<reply>();
  reply_ = http::server::reply::stock_reply(reply::not_found);
  return reply_;
}

std::unique_ptr<reply> RequestHandlerProxy::getReply(std::string headers,
                                                     std::string content,
                                                     const request &request_) {
  std::unique_ptr<reply> reply_ = std::make_unique<reply>();

  std::vector<std::string> vec_of_headers_tmp;
  boost::split(vec_of_headers_tmp, headers, [](char c) {
    return (c == '\r' || c == '\n');
  });
  std::vector<std::string> vec_of_headers;
  for (std::string token : vec_of_headers_tmp) {
    if (token.length() != 0)
      vec_of_headers.push_back(token);
  }

  reply_->headers.resize(vec_of_headers.size() - 1);
  for (int i = 1; i < vec_of_headers.size(); i++) {
    std::string header_cur = vec_of_headers[i];
    for (int j = 0; j < header_cur.size(); j++) {
      if (header_cur.at(j) == ':') {
        reply_->headers[i - 1].name = header_cur.substr(0, j);
        if (j + 1 < header_cur.size() && header_cur.at(j + 1) == ' ')
          reply_->headers[i - 1].value = header_cur.substr(j + 2, header_cur.length());
        else
          reply_->headers[i - 1].value = header_cur.substr(j + 1, header_cur.length());
      }
    }
  }

  std::string status_line = vec_of_headers[0];
  std::vector<std::string> vec_of_status_tmp;
  boost::split(vec_of_status_tmp, status_line, [](char c) {
    return (c == ' ' || c == '\t');
  });
  std::vector<std::string> vec_of_status;
  for (std::string token : vec_of_status_tmp) {
    if (token.length() != 0)
      vec_of_status.push_back(token);
  }
  if (vec_of_status[1] == "200")
    reply_->status = reply::ok;
  else if (vec_of_status[1] == "201")
    reply_->status = reply::created;
  else if (vec_of_status[1] == "202")
    reply_->status = reply::accepted;
  else if (vec_of_status[1] == "204")
    reply_->status = reply::no_content;
  else if (vec_of_status[1] == "300")
    reply_->status = reply::multiple_choices;
  else if (vec_of_status[1] == "301")
    reply_->status = reply::moved_permanently;
  else if (vec_of_status[1] == "302")
    reply_->status = reply::moved_temporarily;
  else if (vec_of_status[1] == "304")
    reply_->status = reply::not_modified;
  else if (vec_of_status[1] == "400")
    reply_->status = reply::bad_request;
  else if (vec_of_status[1] == "401")
    reply_->status = reply::unauthorized;
  else if (vec_of_status[1] == "403")
    reply_->status = reply::forbidden;
  else if (vec_of_status[1] == "404")
    reply_->status = reply::not_found;
  else if (vec_of_status[1] == "500")
    reply_->status = reply::internal_server_error;
  else if (vec_of_status[1] == "501")
    reply_->status = reply::not_implemented;
  else if (vec_of_status[1] == "502")
    reply_->status = reply::bad_gateway;
  else
    reply_->status = reply::service_unavailable;

  reply_->content = content;
  return reply_;
}

std::unique_ptr<reply> RequestHandlerProxy::handleRedirect(const std::string& location_header,
                                                           const request &request_) {
  if (location_header.find("http://") != std::string::npos) {
    std::string host, port, path, query;
    std::size_t host_start = location_header.find("http://") + 7;
    std::size_t path_start = location_header.find("/", host_start);

    std::string host_and_port;
    if (path_start == std::string::npos)
      host_and_port = location_header.substr(host_start);
    else
      host_and_port = location_header.substr(host_start, path_start - host_start);

    std::size_t delimeter_pos = host_and_port.find(":");
    if (delimeter_pos == std::string::npos || delimeter_pos == host_and_port.size() - 1) {
      host = host_and_port;
      port = "80";
    } else {
      host = host_and_port.substr(0, delimeter_pos);
      port = host_and_port.substr(delimeter_pos + 1);
    }

    if (path_start == std::string::npos) {
      path = "/";
    } else {
      std::string relative_url = location_header.substr(path_start);
      parsePathAndQuery(relative_url, path, query);
    }

    request request_next;
    request_next.method = request_.method;
    request_next.uri = path;
    // TODO: set request body to be query
    request_next.http_version_major = request_.http_version_major;
    request_next.http_version_minor = request_.http_version_minor;
    request_next.headers.assign(request_.headers.begin(), request_.headers.end());

    for (int i = 0; i < request_next.headers.size(); i++) {
      if (request_next.headers[i].name == "Host") {
        request_next.headers[i].value = host + ":" + port;
        break;
      }
    }
    return handleRequestHelper(host, port, request_next);
  } else {
    std::string path;
    std::string query;
    std::size_t path_start = location_header.find_last_of(" ");
    if (path_start == std::string::npos || path_start == location_header.size() - 1) {
      path = "/";
    } else {
      std::string relative_url;
      if (location_header[path_start + 1] != '/')
        relative_url = "/" + location_header.substr(path_start + 1);
      else
        relative_url = location_header.substr(path_start + 1);
      parsePathAndQuery(relative_url, path, query);
    }

    request request_next;
    request_next.method = request_.method;
    request_next.uri = path;
    // TODO: set request body to be query
    request_next.http_version_major = request_.http_version_major;
    request_next.http_version_minor = request_.http_version_minor;
    request_next.headers.assign(request_.headers.begin(), request_.headers.end());

    return handleRequestHelper(server_host_, server_port_, request_next);
  }
}

void RequestHandlerProxy::parsePathAndQuery(const std::string& relative_url,
                                            std::string& path,
                                            std::string& query) {
  std::size_t query_pos = relative_url.find("?");
  if (query_pos != std::string::npos) {
    path.append(relative_url.substr(0, query_pos));
    query.append(relative_url.substr(query_pos + 1));
  } else {
    path.append(relative_url);
  }
}
