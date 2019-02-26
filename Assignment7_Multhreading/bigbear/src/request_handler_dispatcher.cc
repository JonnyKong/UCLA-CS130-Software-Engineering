#include "request_handler_dispatcher.h"
#include <boost/log/trivial.hpp>
#include <mutex>

std::unordered_map<int,int> RequestHandlerDispatcher::statusCounter = {{200, 0}, {400, 0}, {404, 0}};

std::mutex mtx_1;

RequestHandlerDispatcher::RequestHandlerDispatcher(NginxConfig& config)
{
    for(auto& nests : config.getNestedParameters())
    {
        std::string uri = nests.second->getFlatParameters().at("location");
        std::string handlerName = nests.first;
        uriToHandlerNameMap_[uri] = handlerName;
        sortedURIs_.push_back(uri);
    }

    std::sort(sortedURIs_.begin(), sortedURIs_.end(), comparePathDepths);

}

int RequestHandlerDispatcher::isPrefix(std::string str1, std::string str2)
{
    // implemented James Kanze's solution at:
    // https://stackoverflow.com/questions/7913835/check-if-one-string-is-a-prefix-of-another
    // if(str1.length() > str2.length()) return -1;
    // for(int i = 0; i < str1.length(); ++i)
    // {
    //     if(str1[i] != str2[i]) return -1;
    // }
    // return ((str1.compare(str2) == 0) ? int(str1.length()) : 0);

    //fix the check prefix function
    if(str2.substr(0, str1.length()) == str1){
      return  str1.length();
    }
    return 0;
}

bool RequestHandlerDispatcher::isSuffix (std::string suffix, std::string fullString)
{
    // implemented kdt's solution at:
    // https://stackoverflow.com/questions/874134/find-if-string-ends-with-another-string-in-c
    if (fullString.length() >= suffix.length())
    {
        return (0 == fullString.compare (fullString.length() - suffix.length(), suffix.length(), suffix));
    }
    else
    {
        return false;
    }
}

bool RequestHandlerDispatcher::uriContainsFileExtension(std::string uri)
{
    // should refactor this so that this easily mirrors the file extensions we are able to serve
    if(isSuffix(".html", uri) ||
        isSuffix(".txt", uri) ||
        isSuffix(".gif", uri) ||
        isSuffix(".jpeg", uri))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool RequestHandlerDispatcher::comparePathDepths(const std::string& str1, const std::string& str2)
{
    int nSubpaths1 = std::count(str1.begin(), str1.end(), '/');
    int nSubpaths2 = std::count(str2.begin(), str2.end(), '/');
    return nSubpaths1 > nSubpaths2;
}

std::string RequestHandlerDispatcher::removeChildPath(std::string path)
{
    std::size_t childPathStart = path.find_last_of('/');
    return path.substr(0, childPathStart);
}

std::string RequestHandlerDispatcher::getLongestMatchingURI(std::string requestURI)
{
    // Remove trailing slashes
    while (requestURI.length() > 1 && requestURI.back() == '/')
        requestURI.pop_back();

    // actually I think there's no need to remove the child path of filename
    // as long as we can find the longest matching prefix
    // by: zephyrwyz (rubberducks)
    if (uriContainsFileExtension(requestURI))
    {
        requestURI = removeChildPath(requestURI);
    }
    // std::cout << "rGLM URI: " << requestURI << std::endl;
    // edge case: request URI is longer than the longest server endpoint

    // if (comparePathDepths(requestURI, sortedURIs_.at(0))) // TODO: Seems incorrect
    // {
    //     return std::string();
    // }

    std::pair<int, std::string> ans(0, std::string(""));
    // find the longest server endpoint with a prefix that matches the request URI
    for (auto &serverEndpointURI : sortedURIs_)
    {
        // std::cout << "rURI: " << requestURI << "|" << std::endl;
        // std::cout << "serverEp: " << serverEndpointURI << "|" << std::endl;
        int tmp = isPrefix(serverEndpointURI, requestURI);
        if ( tmp > ans.first)
        {
            // std::cout << "Got it" << std::endl;
            ans.first = tmp;
            ans.second = serverEndpointURI;
        }
    }
    return ans.second;
}

std::string RequestHandlerDispatcher::getHandlerName(HttpRequest request)
{
    std::string handlerName;
    std::string longestMatchingURI = getLongestMatchingURI(request.requestURI);

    if (longestMatchingURI.empty())
    {
        handlerName = "error";
    }
    else
    {
        handlerName = uriToHandlerNameMap_[longestMatchingURI];
    }
    return handlerName;
}

std::string RequestHandlerDispatcher::dispatchHandler(HttpRequest request,
    HandlerManager* handlerManager, NginxConfig& config)
{
    // handlerName ex) "echo123"
    std::string handlerName = getHandlerName(request);
    std::size_t locationOfNumericChars = handlerName.find_first_of("1234567890");

    if (handlerName.substr(0, locationOfNumericChars).compare("status") == 0)
    {
        config.getNestedParameters()[handlerName]->addFlatParam(std::string("statusInfo"), renderStatusInfo(config));
    }

    // Locks access to handler manager and statusCounter
    std::lock_guard<std::mutex> lock(mtx_1);

    std::unique_ptr<RequestHandler> handler = handlerManager->createByName(
        handlerName.substr(0, locationOfNumericChars),
        *config.getNestedParameters()[handlerName],
        config.getFlatParameters().at("root"));

    std::unique_ptr<HttpResponse> response = handler->HandleRequest(request);

    std:string responseString = response->buildHttpResponse();

    // renew status counter
    statusCounter[response->errorCode] += 1;

    // c++ question: we don't need to explicitly delete a unique ptr, correct? Will
    // it delete itself when we go out of the scope of the function?
    // handler.reset();
    // printf("response string: %s\n", responseString.c_str());
    return responseString;
}

std::string RequestHandlerDispatcher::renderStatusInfo(NginxConfig& config)
{
    // Add status information to config, for status handler
    string statusInfo = "Valid handlers: URL prefix\n-------\n";
    for (auto stat : config.getNestedParameters())
    {
        for (auto a : stat.second->getFlatParameters()){
            if (a.first.compare("location") == 0)
            {
                statusInfo += stat.first;
                statusInfo += ": ";
                statusInfo += a.second;
                statusInfo += "\n";
            }
        }
    }
    std::lock_guard<std::mutex> lock(mtx_1);
    statusInfo += "\n================\nStatus Code: Triggered Count\n-------\n";
    for (auto stat : statusCounter)
    {
        statusInfo += std::to_string(stat.first);
        statusInfo += ": ";
        statusInfo += std::to_string(stat.second);
        statusInfo += "\n";
    }
    return statusInfo;
}
