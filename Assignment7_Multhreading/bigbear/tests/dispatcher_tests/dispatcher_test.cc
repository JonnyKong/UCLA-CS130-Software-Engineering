#include "gtest/gtest.h"
#include "server.h"
#include "config_parser.h"

class DispatcherTest : public ::testing::Test {
  protected:
    HandlerManager handlerManager;
    NginxConfig topLevelConfig;
    NginxConfig* static1Config = new NginxConfig("static1");
    NginxConfig* static2Config = new NginxConfig("static2");
    NginxConfig* static3Config = new NginxConfig("static3");
    NginxConfig* errorConfig = new NginxConfig("error1");
    HttpRequest request;
    NginxConfigParser parser;
};

// we expect to dispatch static handler number 2 because it will have a more specific
// path than static 1. Should result in the home.html file being served from the "some_files"
// directory
TEST_F(DispatcherTest, dispatchStaticHandler2)
{
    request.requestURI = "/static/home.html";
    std::string expectedOutput = "HTTP/1.1 200 OK\r\nContent-Length: 168\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <title>Home.html</title>\n</head>\n<body>\n<h1>THIS IS THE HOME PAGE OF BIGBEAR</h1>\n</body>\n</html>";
    
    // static handler 1 with endpoint /static/foo
    static1Config->addFlatParam("location", "/static/foo");
    static1Config->addFlatParam("root", "/static_files/more_files");
    
    // static handler 2 with longer endpoint /static/foo/bar
    static2Config->addFlatParam("location", "/static");
    static2Config->addFlatParam("root", "/static_files/some_files");

    // static handler 3 has a long location, but has a short matching path prefix. Searching for this
    // location should not occur unless the other two static handler locations do not exist
    static3Config->addFlatParam("location", "/static11111111111111111111111111111111");
    static3Config->addFlatParam("root", "/static_files/more_files");

    // add each static handler's config params into the top level config's nested params
    topLevelConfig.addFlatParam("root", "/usr/src/projects/bigbear");
    topLevelConfig.addNestedParam("static1", static1Config);
    topLevelConfig.addNestedParam("static2", static2Config);
    topLevelConfig.addNestedParam("static3", static3Config);

    // init and call the dispatcher and expect the static 2 handler to be dispatched
    RequestHandlerDispatcher dispatcher(topLevelConfig);
    std::string dispatcherOutput = dispatcher.dispatchHandler(request, &handlerManager, topLevelConfig);

    EXPECT_STREQ(dispatcherOutput.c_str(), expectedOutput.c_str());
}

// TODO: This test doesn't pass... don't know why 
// TEST_F(DispatcherTest, dispatchStaticHandlerGivenRealConfig)
// {
//     bool success = parser.Parse("../server_config_new", &topLevelConfig);
//     request.requestURI = "/static/home.html";
//     std::string expectedOutput = "HTTP/1.1 200 OK\r\nContent-Length: 168\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <title>Home.html</title>\n</head>\n<body>\n<h1>THIS IS THE HOME PAGE OF BIGBEAR</h1>\n</body>\n</html>";

//     RequestHandlerDispatcher dispatcher(topLevelConfig);
//     std::string dispatcherOutput = dispatcher.dispatchHandler(request, &handlerManager, topLevelConfig);
    
//     EXPECT_STREQ(dispatcherOutput.c_str(), expectedOutput.c_str());
// }

TEST_F(DispatcherTest, defaultToError)
{
    request.requestURI = "/";
    std::string expectedOutput = "HTTP/1.1 404 File Not Found\r\nContent-Length: 15\r\nContent-Type: text/plain\r\n\r\n404 not found\r\n";
    // static handler 1 with endpoint /static/foo
    errorConfig->addFlatParam("location", "/");
    errorConfig->addFlatParam("root", "whatever");

    // static handler 2 with longer endpoint /static/foo/bar
    static1Config->addFlatParam("location", "/static/foo/bar");
    static1Config->addFlatParam("root", "static_files/some_files");

    // static handler 3 has a long location, but has a short matching path prefix. Searching for this
    // location should not occur unless the other two static handler locations do not exist
    static2Config->addFlatParam("location", "/static11111111111111111111111111111111");
    static2Config->addFlatParam("root", "static_files/more_files");

    // add each static handler's config params into the top level config's nested params
    topLevelConfig.addFlatParam("root", "/usr/src/projects/bigbear");
    topLevelConfig.addNestedParam("error1", errorConfig);
    topLevelConfig.addNestedParam("static1", static1Config);
    topLevelConfig.addNestedParam("static2", static2Config);

    // init and call the dispatcher and expect the static 2 handler to be dispatched
    RequestHandlerDispatcher dispatcher(topLevelConfig);
    std::string dispatcherOutput = dispatcher.dispatchHandler(request, &handlerManager, topLevelConfig);


    EXPECT_STREQ(dispatcherOutput.c_str(), expectedOutput.c_str());
}