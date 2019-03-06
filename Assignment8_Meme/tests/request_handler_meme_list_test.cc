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

class MockRequestHandlerMemeList : public RequestHandlerMemeList {
public:
    MockRequestHandlerMemeList(const NginxConfig &config) {
        database_name = std::string("../assets/meme_test.db");
        remove(database_name.c_str());  // Doesn't matter if failed
        maybeInit(database_name);
    }
    ~MockRequestHandlerMemeList() {
        (database_name.c_str());
    }
};

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

TEST(RequestHandlerMemeListTest, SelectionTest) {
    NginxConfig empty_config;
    MockRequestHandlerMemeCreate mock_create_hdlr(empty_config);
    MockRequestHandlerMemeList mock_list_hdlr(empty_config);
    int count = 3;
    int id;

    MemeEntry entry1("image_1", "top_1", "bot_1");
    mock_create_hdlr.insertToStorage(entry1, id);
    MemeEntry entry2("image_1", "top_2", "bot_2");
    mock_create_hdlr.insertToStorage(entry2, id);
    MemeEntry entry3("image_1", "top_3", "bot_3");
    mock_create_hdlr.insertToStorage(entry3, id);

    std::vector<MemeEntry> meme_list = mock_list_hdlr.selectAllMeme();
    EXPECT_TRUE(meme_list.size() == count);
}
