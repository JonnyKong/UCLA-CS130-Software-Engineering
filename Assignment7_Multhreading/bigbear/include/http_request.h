#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <unordered_map>
#include <string>

#include "http_request_enums.h"

using namespace httpRequestEnum;

class HttpRequest
{
public:
    std::string unparsedRequestString;
    // These are the components of the request
    httpMethod requestMethod;
    std::string httpVersion;
    std::string requestURI;
    std::string requestBody;
    bool isComplete;

    // Map of header field and its corresponding value
    std::unordered_map<httpHeaderFields, std::string> headerFields;

    // Parses the request, sets components, and return true if valid
    bool parseHttpRequest(std::string requestString);
    std::string getRequestURI()
    {
        return requestURI;
    }

    // If we have previously parsed an incomplete HTTP request, we 
    // call this function when we receive the rest of the HTTP request
    // (or if we receive another piece of the request)
    bool finishParsingRequest(std::string partialRequest);

private:

    // Helper functions set each component within parseHttpRequest
    // Return true if successful
    bool setMethod(std::string);
    bool setURI(std::string);
    bool setVersion(std::string);
    bool setHeaderFields(std::string, std::string);
    bool setBody(std::string);
};


#endif //HTTP_PARSER_H
