#include "static_file_request_handler.h"
#include "http_response.h"
#include <boost/log/trivial.hpp>

using std::ifstream;
using std::ios;

StaticFileRequestHandler::StaticFileRequestHandler(
        const std::unordered_map<std::string, std::string> staticPathMap, std::string rootPath) :
        RequestHandler()
{
    validURIMap = staticPathMap;
    root = rootPath;
    status = 200;
}

void StaticFileRequestHandler::setFileType()
{
    if(fileName.find(".txt") != std::string::npos)
    {
        fileType = TXT;
    }
    else if(fileName.find(".html") != std::string::npos)
    {
        fileType = HTML;
    }
    else if(fileName.find(".gif") != std::string::npos)
    {
        fileType = GIF;
    }
    else if(fileName.find(".jpg") != std::string::npos)
    {
        fileType = JPEG;
    }
}

void StaticFileRequestHandler::clearVariables()
{
    fileName = "";
    pathToFile = "";
    URI = "";
    status = 200;
}

void StaticFileRequestHandler::setURIAndFileName(std::string uriWithFileName)
{
    std::cout << "uriWithFileName:" << uriWithFileName << std::endl;
    if(uriWithFileName.length() > 1)
    {
        std::string::iterator it = uriWithFileName.begin();
        if (*it == '/')
        {
            ++it;
            URI.push_back('/');
            for (; *it != '/' && it != uriWithFileName.end(); ++it)
            {
                URI.push_back(*it);
            }
            if (it != uriWithFileName.end())
            {
                ++it;
                for (; it != uriWithFileName.end(); ++it)
                {
                    fileName.push_back(*it);
                }
            }
        }
    }
    std::cout << "uri:" << URI << std::endl;
    std::cout << "filename:" << fileName << std::endl;
}

void StaticFileRequestHandler::setPathToFile()
{
    std::string path = root + '/' + validURIMap["root"];
    pathToFile = path + '/' + fileName;
    std::cout << "PATH TO FILE: " << pathToFile << std::endl;
}

bool StaticFileRequestHandler::doesFileExist()
{
    if(fileName.empty()) { return false; }
    ifstream fileToServe(pathToFile);
    return fileToServe.good();
}

std::string StaticFileRequestHandler::createResponseHeader(unsigned contentLength)
{
    std::string contentLengthStr(std::to_string(contentLength));
    std::string mimeType;
    switch (fileType)
    {
        case TXT:
            mimeType = "text/plain";
            break;
        case HTML:
            mimeType = "text/html";
            break;
        case GIF:
            mimeType = "image/gif";
            break;
        case JPEG:
            mimeType = "image/jpeg";
            break;
    }
    std::map<std::string,std::string> headers { {"Content-Type", mimeType},
                                                {"Content-Length", contentLengthStr}};

    HttpResponse response;

    return response.buildHttpResponse(status, headers);
}


void StaticFileRequestHandler::setResponse(HttpResponse& response, std::string& fileStr)
{
    std::string contentLengthStr(std::to_string(fileStr.length()));
    std::string mimeType;
    switch (fileType)
    {
        case TXT:
            mimeType = "text/plain";
            break;
        case HTML:
            mimeType = "text/html";
            break;
        case GIF:
            mimeType = "image/gif";
            break;
        case JPEG:
            mimeType = "image/jpeg";
            break;
    }
    std::map<std::string,std::string> headers { {"Content-Type", mimeType},
                                                {"Content-Length", contentLengthStr}};

    response.setHttpResponse(status, headers, fileStr);
}

void StaticFileRequestHandler::initRequestVariables()
{
    setPathToFile();
    if(!doesFileExist())
    {
        BOOST_LOG_TRIVIAL(debug) << "Invalid File Request:" << pathToFile;
        fileName = "404error.html";
        pathToFile = root + "/static_files/404error.html";
        status = 404;
    }
    setFileType();
}

// https://stackoverflow.com/questions/25308676/send-file-via-boost-asio-tcp-how-to-stream-char-array-to-socket
std::string StaticFileRequestHandler::fileToString(const std::string &name)
{
    using namespace std;
    ifstream fl(name);
    fl.seekg( 0, ios::end );
    size_t len = fl.tellg();
    string ret;
    ret.resize(len);
    fl.seekg(0, ios::beg);
    if (len)
        fl.read(&ret[0], len);
    fl.close();
    // returns file as vector<char>
    return move(ret);
}

std::unique_ptr<RequestHandler> StaticFileRequestHandler::create(const NginxConfig& config, const std::string& root_path)
{
    StaticFileRequestHandler staticHandler(config.getFlatParameters(), root_path);
    return std::make_unique<StaticFileRequestHandler>(staticHandler);
}

std::unique_ptr<HttpResponse> StaticFileRequestHandler::HandleRequest(const HttpRequest &request)
{
    setURIAndFileName(request.requestURI);
    initRequestVariables();
    std::string fileStr = fileToString(pathToFile);

    HttpResponse response;
    setResponse(response, fileStr);
    return std::make_unique<HttpResponse>(response);
}
