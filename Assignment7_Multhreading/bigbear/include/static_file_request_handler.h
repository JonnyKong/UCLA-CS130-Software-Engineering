#ifndef STATIC_FILE_REQUEST_HANDLER_H
#define STATIC_FILE_REQUEST_HANDLER_H

#include "request_handler.h"
#include "http_response.h"
#include "fstream"
#include "config_parser.h"
#include <memory>

class StaticFileRequestHandler : public RequestHandler
{
public:
    static std::unique_ptr<RequestHandler> create(const NginxConfig& config, const std::string& root_path);
    std::unique_ptr<HttpResponse> HandleRequest(const HttpRequest &request);

    StaticFileRequestHandler() {}; // for testing
    StaticFileRequestHandler(const std::unordered_map<std::string, std::string> staticPathMap, std::string rootPath);

//private:

    enum FileType
    {
        TXT = 0,
        HTML = 1,
        GIF = 2,
        JPEG = 3
    };

    std::unordered_map<std::string, std::string> validURIMap;
    std::string root;
    std::string fileName;
    std::string pathToFile;
    std::string URI;
    FileType fileType;
    bool fileExists;


    void setFileType();
    void setURIAndFileName(std::string uriWithFileName);
    void setPathToFile();
    void clearVariables();

    // https://stackoverflow.com/questions/25308676/send-file-via-boost-asio-tcp-how-to-stream-char-array-to-socket
    std::string fileToString(const std::string &name);
    std::string createResponseHeader(unsigned contentLength);
    void setResponse(HttpResponse& response, std::string& fileStr);
    bool doesFileExist();
    void initRequestVariables();

};


#endif //STATIC_FILE_REQUEST_HANDLER_H
