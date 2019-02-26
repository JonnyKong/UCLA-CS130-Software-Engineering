#include "gtest/gtest.h"
#include "action_request_handler.h"
#include "http_request.h"
#include "http_response.h"
#include "config_parser.h"

//using namespace std;
//
//class ActionHandlerTest : public ::testing::Test {
//protected:
//    unordered_map<ServerAction, string> serverActionMap {{ACTION_ECHO, "/"}};
//    // serverActionMap.insert({ACTION_ECHO, "/"});
//
//    string echoHTTPmessage = "GET / HTTP/1.1\r\nUser-Agent: nc/0.0.1\r\nHost: 127.0.0.1\r\nAccept: */*\r\n\r\n";
//
//    HttpRequest req;
//};
//
//TEST_F (ActionHandlerTest, ResetHandlerAttributes)
//{
//    ActionRequestHandler actionRequestHandler(serverActionMap);
//    actionRequestHandler.clearVariables();
//    EXPECT_STREQ("200", actionRequestHandler.status.c_str());
//    EXPECT_STREQ("", actionRequestHandler.URI.c_str());
//}
//
//TEST_F (ActionHandlerTest, CanHandleValidRequest)
//{
//    ActionRequestHandler actionRequestHandler(serverActionMap);
//    req.parseHttpRequest(echoHTTPmessage);
//    EXPECT_TRUE(actionRequestHandler.canHandleRequest(req));
//}
//
//TEST_F (ActionHandlerTest, HandleRequest)
//{
//    ActionRequestHandler actionRequestHandler(serverActionMap);
//    req.parseHttpRequest(echoHTTPmessage);
//    string expectedResponse = actionRequestHandler.echoRequest(req);
//    EXPECT_STREQ(expectedResponse.c_str(), actionRequestHandler.handleRequest(req).c_str());
//}