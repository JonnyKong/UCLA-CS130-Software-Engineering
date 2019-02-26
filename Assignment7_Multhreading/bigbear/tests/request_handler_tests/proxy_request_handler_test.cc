#include "gtest/gtest.h"
#include "proxy_request_handler.h"
#include "http_request.h"
#include "nginx_config.h"
#include <unordered_map>
#include "boost/filesystem.hpp"

using namespace std;

class ProxyRequestHandlerTest : public ::testing::Test {
protected:
    unordered_map<string, string> pathMap { {"dest", "www.rubberducks.cs130.org"} };

    HttpRequest req;

};

TEST_F(ProxyRequestHandlerTest, ProxyNotFound) {

    ProxyRequestHandler proxyHandler(pathMap, "../..");
    req.requestURI = "/rubberducks/pathnotexist";
    std::unique_ptr<HttpResponse> res = proxyHandler.HandleRequest(req);
    EXPECT_TRUE(res->errorCode == 404);
}

TEST_F(ProxyRequestHandlerTest, ProxySuccessful) {

    ProxyRequestHandler proxyHandler(pathMap, "../..");
    req.requestURI = "/rub/echo";
    std::unique_ptr<HttpResponse> res = proxyHandler.HandleRequest(req);
    EXPECT_TRUE(res->errorCode == 200);
}

TEST_F(ProxyRequestHandlerTest, ProxyRedirect) {
    // The test will only pass after the server has been redeployed to incorporate redirect handler.
    // TODO: Change "redirectMap" to the fisrt definition when server deployment is successful

    unordered_map<string, string> redirectMap { {"dest", "35.247.29.163/redirect"} };

    ProxyRequestHandler proxyHandler(redirectMap, "../..");
    req.requestURI = "/moved_echo";
    std::unique_ptr<HttpResponse> res = proxyHandler.HandleRequest(req);
    cout << "Status Code" <<res -> errorCode << endl;

    EXPECT_TRUE(res->errorCode == 200);
    // TODO: Change errorCode assertion to 200 when server deployment is successful
    // EXPECT_TRUE(res->errorCode == 200);
}
