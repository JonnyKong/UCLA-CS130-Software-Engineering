#include "nginx_config.h"
#include "handler_manager.h"
#include "http_request.h"
#include "http_response.h"
#include "request_handler.h"
#include <unordered_map>

// Server will have a RequestHandlerDispatcher, which is used by session
class RequestHandlerDispatcher
{
public:
    // constructor will initialize mappings between uri endpoints and handler names
    RequestHandlerDispatcher(NginxConfig& config);

    // called during a session to construct, use, and finally destroy a request handler. Returns
    // raw string to write back to socket
    std::string dispatchHandler(HttpRequest request, HandlerManager* handlerManager, NginxConfig& config);

private:
    // maps endpoints to proper handler names
    std::unordered_map<std::string, std::string> uriToHandlerNameMap_;

    // a vector of the server's URI endpoints, sorted in descending order
    std::vector<std::string> sortedURIs_;

    // auxilliary functions used by the dispatcher
    int isPrefix(std::string str1, std::string str2);
    bool isSuffix(std::string fullString, std::string suffix);
    bool uriContainsFileExtension(std::string uri);
    static bool comparePathDepths(const std::string& str1, const std::string& str2);

    // takes in a path such as '/foo/bar/file.txt' and returns '/foo/bar'
    std::string removeChildPath(std::string);

    // returns a string representing the longest matching server endpoint to the request's URI
    std::string getLongestMatchingURI(std::string requestURI);

    // takes in an HttpRequest and returns the name of the proper handler. Calls getLongestMatchingURI
    std::string getHandlerName(HttpRequest request);

    // Status Counter // TODO: maybe we can set up an independent class for counter
    static std::unordered_map<int, int> statusCounter; // TODO: maybe we can let it accomodate status code dynamically

    std::string renderStatusInfo(NginxConfig& config);
};