#include "gtest/gtest.h"
#include "config_parser.h"
#include "request_handler/request_handler_proxy.h"


//comment out request handler test written by team Nuke at the moment

// TEST(RequestHandlerProxyTest, HandleResquestTest) {
//   NginxConfigParser config_parser;
//   NginxConfig config;
//   NginxConfig proxy_handler_config;
//   NginxConfig handler_config;
//   bool success = config_parser.Parse("../conf/test.conf", &config);
//   for (int i = 0; i < config.statements_.size(); ++i) {
//     if ((config.statements_[i])->tokens_[0] == "handler" && (config.statements_[i])->tokens_[1] == "proxy")
//       handler_config = *(config.statements_[i])->child_block_;
//     for (int j = 0; j < handler_config.statements_.size(); ++j) {
//       if (handler_config.statements_[j]->tokens_[0] == "location" && handler_config.statements_[j]->tokens_[1] == "/nuke") {
//         proxy_handler_config = handler_config;
//       }
//     }
//   }
//   request request_;
//   request_.method = "GET";
//   request_.uri = "/nuke/text/index.txt";
//   request_.http_version_major = 1;
//   request_.http_version_minor = 1;
//   RequestHandlerProxy request_handler_proxy(proxy_handler_config);
//   std::unique_ptr<reply> reply_ = request_handler_proxy.handleRequest(request_);
//   EXPECT_EQ(reply_->status, reply::ok);
//   EXPECT_EQ(reply_->content, "Nuke Server");
//   EXPECT_EQ(reply_->headers[0].name, "Content-Type");
//   EXPECT_EQ(reply_->headers[0].value, "text/plain");
//   EXPECT_EQ(reply_->headers[1].name, "Content-Length");
//   EXPECT_EQ(reply_->headers[1].value, "11");
// }
