#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "gtest/gtest.h"
#include "request_handler/request_handler_meme_form.h"
#include "request_handler/request_handler_meme_list.h"
#include "request_handler/request_handler_meme_view.h"
#include "request_handler/request_handler_meme_create.h"
#include "http/request.h"
#include "http/request_parser.h"
#include "config_parser.h"
#include "http/reply.h"
#include "request_handler_dispatcher.h"  // Include typedefs
#include "http/mime_types.h"

class MockRequestHandlerMemeCreate : public RequestHandlerMemeCreate {
public:
    MockRequestHandlerMemeCreate(const NginxConfig &config) {
        database_name = std::string("../assets/meme_test.db");
        remove(database_name.c_str());  // Doesn't matter if failed
        maybeInit(database_name);
    }
    ~MockRequestHandlerMemeCreate() {
        remove(database_name.c_str());
    }
};

TEST(RequestHandlerMemeCreateTest, InsertionTest) {
    NginxConfig empty_config;
    MockRequestHandlerMemeCreate mock_hdlr(empty_config);
    int id;
    MemeEntry entry("image_1", "top_1", "top_2");
    std::string result = mock_hdlr.insertToStorage(entry, id);
    EXPECT_TRUE(result == std::string("SUCCESS"));
}

TEST(RequestHandlerMemeCreateTest, DuplicateInsertionTest) {
    NginxConfig empty_config;
    MockRequestHandlerMemeCreate mock_hdlr(empty_config);
    int id;
    MemeEntry entry("image_1", "top_1", "top_2");

    std::string result_1 = mock_hdlr.insertToStorage(entry, id);
    std::string result_2 = mock_hdlr.insertToStorage(entry, id);

    EXPECT_TRUE(result_1 == std::string("SUCCESS"));
    EXPECT_TRUE(result_2 != std::string("SUCCESS"));
}

TEST(RequestHandlerMemeCreateTest, ParseRESTParamsTest) {
    NginxConfig empty_config;
    MockRequestHandlerMemeCreate mock_hdlr(empty_config);

    std::string uri = "foo/bar?param1=value1&param2=value2";
    std::map<std::string, std::string> params = mock_hdlr.parseRESTParams(uri);
    EXPECT_TRUE(params.size() == 2);
    EXPECT_TRUE(params["param1"] == "value1");
    EXPECT_TRUE(params["param2"] == "value2");
}