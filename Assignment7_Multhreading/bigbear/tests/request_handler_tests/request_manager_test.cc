#include <handler_manager.h>
#include <http_response.h>
#include <http_request.h>
#include "gtest/gtest.h"

TEST(HandlerManagerTest, ErrorHandler)
{
    NginxConfig config;
    std::string root_path = "";
    HandlerManager manager;
    std::unique_ptr<RequestHandler> errorHandler = manager.createByName("400", config, root_path);

    HttpRequest req;
    std::unique_ptr<HttpResponse> res = errorHandler->HandleRequest(req);
    std::string expectedOutput = "HTTP/1.1 404 File Not Found\r\n"
                                 "Content-Length: 15\r\n"
                                 "Content-Type: text/plain\r\n"
                                 "\r\n"
                                 "404 not found\r\n";
    EXPECT_TRUE(res -> buildHttpResponse() == expectedOutput);
}

TEST(HandlerManagerTest, StaticHandler)
{
    // create the handler
    NginxConfig config;
    config.addFlatParam("root", "static_files/some_files");
    std::string root_path = "../..";
    HandlerManager manager;
    std::unique_ptr<RequestHandler> staticHandler = manager.createByName("static", config, root_path);

    // create fake request
    HttpRequest request;
    request.requestURI = "/static/home.html";

    // call handleRequest and verify the response
    std::unique_ptr<HttpResponse> response = staticHandler->HandleRequest(request);
    EXPECT_TRUE(response -> errorCode == 200);

    // make sure the header contains the mime-type text/html
    EXPECT_TRUE(response -> headers["Content-Type"] == "text/html");

    // make sure the body contains "THIS IS THE HOME PAGE OF BIGBEAR"
    EXPECT_TRUE(response -> body.find("THIS IS THE HOME PAGE OF BIGBEAR") != std::string::npos);
    std::cerr << response -> body;
}

TEST(HandlerManagerTest, ActionHandler)
{
    // create the handler
    NginxConfig config;
    std::string root_path = "../..";
    HandlerManager manager;
    std::unique_ptr<RequestHandler> actionHandler = manager.createByName("echo", config, root_path);

    // create fake request
    HttpRequest request;
    request.requestURI = "/echo";
    request.unparsedRequestString = "I am testing the echo handler";

    // call handleRequest and verify the response
    std::unique_ptr<HttpResponse> response = actionHandler->HandleRequest(request);
    EXPECT_TRUE(response -> errorCode == 200);

    // make sure the header contains the mime-type text/plain
    EXPECT_TRUE(response -> headers["Content-Type"] == "text/plain");

    // make sure the response body contains the request  
    EXPECT_TRUE(response -> body.find(request.unparsedRequestString) != std::string::npos);
    std::cerr << response -> buildHttpResponse();
}

TEST(HandlerManagerTest, StatusHandler)
{
    // create the handler
    NginxConfig config;
    string str1("statusInfo");
    string statusInfo = "200: 0";
    config.addFlatParam(str1, statusInfo);
    std::string root_path = "../..";
    
    HandlerManager manager;
    std::unique_ptr<RequestHandler> actionHandler = manager.createByName("status", config, root_path);

    // create fake request
    HttpRequest request;
    request.requestURI = "/status";
    std::string targetString = "200: ";

    // call handleRequest and verify the response
    std::unique_ptr<HttpResponse> response = actionHandler->HandleRequest(request);
    EXPECT_TRUE(response -> errorCode == 200);

    // make sure the header contains the mime-type text/plain
    EXPECT_TRUE(response -> headers["Content-Type"] == "text/plain");

    // make sure the response body contains the request  
    EXPECT_TRUE(response -> body.find(targetString) != std::string::npos);
    std::cerr << response -> buildHttpResponse();
}
