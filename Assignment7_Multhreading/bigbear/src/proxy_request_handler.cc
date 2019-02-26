#include <iostream>
#include <cassert>

#include "proxy_request_handler.h"

using boost::asio::buffers_begin;

ProxyRequestHandler::ProxyRequestHandler(
    std::unordered_map<std::string, std::string> config_params, std::string destPath):
    RequestHandler()
{
    dest = config_params["dest"];
    port = "80";
    size_t cursor = dest.find_first_of("/");
    if (cursor != std::string::npos){
        uriPath = dest.substr(cursor);
        dest = dest.substr(0, cursor);
    }
    // Remove trailing slashes
    while (uriPath.length() > 1 && uriPath.back() == '/')
        uriPath.pop_back();

}

void ProxyRequestHandler::setURI(std::string request_uri)
{
    std::cout << "request_uri: " +  request_uri << std::endl;
    if(request_uri.length() > 1)
    {
        std::string::iterator it = request_uri.begin();
        if (*it == '/')
        {
            ++it;
            uriPath.push_back('/');
            for (; *it != '/' && it != request_uri.end(); ++it);
            if (it != request_uri.end())
            {
                ++it;
                for (; it != request_uri.end(); ++it)
                {
                    uriPath.push_back(*it);
                }
            }
        }
    }
}

std::unique_ptr<RequestHandler> ProxyRequestHandler::create(const NginxConfig& config, const std::string& destPath){
    ProxyRequestHandler proxyHandler(config.getFlatParameters(), destPath);
    return std::make_unique<ProxyRequestHandler>(proxyHandler);
}

void ProxyRequestHandler::sendRequestToDest(std::string dest, std::string port, std::string uriPath, unsigned int &status, std::map<std::string,std::string> &headers,std::string &body ){
      if (uriPath.size() >= 2 && uriPath.back() == '/')
        uriPath.pop_back();

      std::cout << "Request URI after set: " << uriPath << "\n";
      std::cout << "dest server name: " << dest << "\n";
      std::cout << "port: " << port << "\n";

      //test for sending request to dest server
      boost::asio::io_service io_service;

      // Get a list of endpoints corresponding to the server name.
      tcp::resolver resolver(io_service);
      tcp::resolver::query query(dest, port);
      tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

      // Try each endpoint until we successfully establish a connection.
      tcp::socket socket(io_service);
      boost::asio::connect(socket, endpoint_iterator);

      // Form the request. We specify the "Connection: close" header so that the
      // server will close the socket after transmitting the response. This will
      // allow us to treat all data up until the EOF as the content.
      boost::asio::streambuf req;
      std::ostream request_stream(&req);
      request_stream << "GET " << uriPath << " HTTP/1.0\r\n";
      request_stream << "Host: " << dest << "\r\n";
      request_stream << "Accept: */*\r\n";
      request_stream << "Connection: close\r\n\r\n";

      // Send the request.
      boost::asio::write(socket, req);
      // Read the response status line. The response streambuf will automatically
      // grow to accommodate the entire line. The growth may be limited by passing
      // a maximum size to the streambuf constructor.
      boost::asio::streambuf response;
      std::cout << "debugging" << "\n";
      auto bufs = response.data();
      std::string debug_string(buffers_begin(bufs), buffers_begin(bufs) + response.size());
      std::cout << debug_string << std::endl;
      boost::asio::read_until(socket, response, "\r\n");


      // Check that response is OK.
      std::istream response_stream(&response);
      std::string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      std::string status_message;
      std::getline(response_stream, status_message);
      status = status_code;


      // Read the response headers, which are terminated by a blank line.
      boost::asio::read_until(socket, response, "\r\n\r\n");

      // Process the response headers.
      headers.clear();
      std::string header;
      while (std::getline(response_stream, header) && header != "\r"){
        std::string token = header.substr(0, header.find(":"));
        headers[token] = header.substr(header.find(":")+2);
        // std::cout << token << ":" << header.substr(header.find(":")+2) << std::endl;
        // std::cout << "header: " <<  header << "\n";
      }
      //
      // std::cout << "\n";

      body = "";
      std::string content;
      // Write whatever content we already have to output.
      if (response.size() > 0){
          //write the reponse content to string
          std::getline(response_stream, content);
      }
      body += content;

      // Read until EOF, writing data to output as we go.
      boost::system::error_code error;
      std::string rest_response;
      while (boost::asio::read(socket, response,
             boost::asio::transfer_at_least(1), error)) {
          auto bufs = response.data();
          std::string content_string(buffers_begin(bufs), buffers_begin(bufs) + response.size());
          rest_response = content_string;
      }
      body += rest_response;
}

std::unique_ptr<HttpResponse> ProxyRequestHandler::HandleRequest(const HttpRequest &request){
    //
    std::cout << "Request URI before set: " << request.requestURI << "\n";
    setURI(request.requestURI);

    unsigned int status;
    std::map<std::string,std::string> headers;
    std::string body;

    sendRequestToDest(dest,port,uriPath, status,headers, body);

    //handle 301 and 302 redirection
    if (status == 301 || status == 302){
        std::string location = headers["Location"];
        std::cout << "Location: " << location << std::endl;
        if (location.substr(0, 8) == std::string("https://"))
            location = location.substr(8, location.size() - 8);
        else if (location.substr(0, 7) == std::string("http://"))
            location = location.substr(7, location.size() - 7);
        std::cout << "Location: " << location << std::endl;
        size_t cursor = location.find_first_of("/");    
        if (cursor != std::string::npos) {
            uriPath = "/" + location.substr(cursor + 1);
            // for some reason, you have to get rid of the last character
            uriPath = uriPath.substr(0, uriPath.length() - 1);
            dest = location.substr(0, cursor);
            size_t cursor_port = dest.find_first_of(":");
            if (cursor_port != std::string::npos) {
                port = dest.substr(cursor_port + 1);
                dest = dest.substr(0, cursor_port);
            }
        } else {
            dest = location;
            uriPath = "/";
            size_t cursor_port = dest.find_first_of(":");
            if (cursor_port != std::string::npos) {
                port = dest.substr(cursor_port + 1);
                port.pop_back();
                dest = dest.substr(0, cursor_port);
            }
        }
        std::cout << "Dest: " << dest << std::endl;
        std::cout << "uriPath: " << uriPath << std::endl;
        sendRequestToDest(dest, port, uriPath, status,headers, body);
    }
    std::cout << "status_code: " << status << std::endl;
    for(auto it = headers.begin(); it!=headers.end(); it++){
         std::cout << "headers: " << it->first  << ":" <<it->second << std::endl;
    }

    HttpResponse res;
    res.setHttpResponse(status, headers, body);
    return std::make_unique<HttpResponse>(res);
}
