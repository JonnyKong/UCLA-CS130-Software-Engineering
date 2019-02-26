#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <map>

class HttpResponse
{
public:
    // I think buildHttpResponse is moving to dispatcher?
    // TODO: remove this function after all components are set.
    std::string buildHttpResponse(int status, std::map<std::string,std::string> headers = {{}},
                                  std::string body = "");
    std::string buildHttpResponse();
    void setHttpResponse(int errorCode, std::map<std::string,std::string> headers = {}, std::string body = "");
    int errorCode;
    std::string mimeType;
    std::map<std::string,std::string> headers;
    std::string body;

private:
    // Helper functions to build the response within buildHttpResponse()
    std::string buildStatus(int);
    std::string buildHeaders(std::string, std::string);
    std::string buildBody(std::string);

};


#endif //HTTP_RESPONSE_H
