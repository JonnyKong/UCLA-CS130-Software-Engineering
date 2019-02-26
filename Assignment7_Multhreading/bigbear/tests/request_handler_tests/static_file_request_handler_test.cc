#include "gtest/gtest.h"
#include "static_file_request_handler.h"
#include "http_request.h"
#include "nginx_config.h"
#include <unordered_map>
#include "boost/filesystem.hpp"

using namespace std;

class StaticFileRequestHandlerTest : public ::testing::Test {
protected:
    unordered_map<string, string> pathMap { {"root", "static_files/some_files"} };

    HttpRequest req;

};

TEST_F(StaticFileRequestHandlerTest, SetURIAndFileName) {
    StaticFileRequestHandler fileHandler(pathMap, "../..");
    req.requestURI = "/static/home.html";
    fileHandler.setURIAndFileName(req.requestURI);
    fileHandler.setFileType();
    fileHandler.setPathToFile();

    EXPECT_TRUE(fileHandler.URI == "/static");
    EXPECT_TRUE(fileHandler.fileName == "home.html");
    EXPECT_TRUE(fileHandler.fileType == StaticFileRequestHandler::FileType::HTML);
    EXPECT_TRUE(fileHandler.pathToFile == "../../static_files/some_files/home.html");
    cout << fileHandler.pathToFile << endl;
}

TEST_F(StaticFileRequestHandlerTest, FileFound) {

    StaticFileRequestHandler fileHandler(pathMap, "../..");
    req.requestURI = "/static/home.html";

    fileHandler.setURIAndFileName(req.requestURI);
    fileHandler.setFileType();
    fileHandler.setPathToFile();
    bool fileExists = fileHandler.doesFileExist();

    EXPECT_TRUE(fileExists);
    cout << "Path to file: " << fileHandler.pathToFile << endl;
//    boost::filesystem::path full_path(boost::filesystem::current_path());
//    cout << "PWD: " << full_path << endl;
}

TEST_F(StaticFileRequestHandlerTest, FileNotFound) {

    StaticFileRequestHandler fileHandler(pathMap, "../..");
    req.requestURI = "/static/doesntExist.txt";
    fileHandler.setURIAndFileName(req.requestURI);
    fileHandler.initRequestVariables();
    EXPECT_TRUE(fileHandler.fileName == "404error.html");
    EXPECT_TRUE(fileHandler.pathToFile == "../../static_files/404error.html");
    EXPECT_TRUE(fileHandler.fileType == StaticFileRequestHandler::FileType::HTML);
    cout << "FILE NAME: " << fileHandler.fileName << endl;
    cout << "Path to file: " << fileHandler.pathToFile << endl;
}

TEST_F(StaticFileRequestHandlerTest, BuildResponseHeader) {

    StaticFileRequestHandler fileHandler(pathMap, "../..");
    req.requestURI = "/static/home.html";
    fileHandler.setURIAndFileName(req.requestURI);
    fileHandler.initRequestVariables();

    string header = fileHandler.createResponseHeader(15);

    string expectedHeader = "HTTP/1.1 200 OK\r\n";
    expectedHeader +=       "Content-Length: 15\r\n";
    expectedHeader +=       "Content-Type: text/html\r\n\r\n";

    EXPECT_TRUE(header == expectedHeader);
    cout << "HEADER: " << header << endl;
    cout << "EXPECTED HEADER: " << expectedHeader << endl;
}

TEST_F(StaticFileRequestHandlerTest, TestHttpParse) {

    StaticFileRequestHandler fileHandler(pathMap, "../..");
    string requestString = "GET /static HTTP/1.1\r\n\r\n";
    HttpRequest request;
    request.parseHttpRequest(requestString);

    EXPECT_TRUE(request.requestURI == "/static");
    cout << "Request URI: " << request.requestURI << endl;
}

