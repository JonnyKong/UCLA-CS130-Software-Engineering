#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <string>

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
    MemeEntry entry("image_1", "top_1", "top_2", 0);
    std::string result = mock_hdlr.insertToStorage(entry, id);
    EXPECT_TRUE(result == std::string("SUCCESS"));
}

TEST(RequestHandlerMemeCreateTest, DuplicateInsertionTest) {
    NginxConfig empty_config;
    MockRequestHandlerMemeCreate mock_hdlr(empty_config);
    int id;
    MemeEntry entry("image_1", "top_1", "top_2", 0);

    std::string result_1 = mock_hdlr.insertToStorage(entry, id);
    std::string result_2 = mock_hdlr.insertToStorage(entry, id);

    EXPECT_TRUE(result_1 == std::string("SUCCESS"));
    EXPECT_TRUE(result_2 != std::string("SUCCESS"));
}

TEST(RequestHandlerMemeCreateTest, ParseRESTParamsTest) {
    NginxConfig empty_config;
    MockRequestHandlerMemeCreate mock_hdlr(empty_config);

    std::string uri = "foo/bar?param1=value1&param2=value2";
    std::map<std::string, std::string> params = parseRESTParams(uri);
    EXPECT_TRUE(params.size() == 2);
    EXPECT_TRUE(params["param1"] == "value1");
    EXPECT_TRUE(params["param2"] == "value2");
}

TEST(RequstHandlerMemeCreateTest, ConcurrencyTest1) {
    NginxConfig empty_config;
    MockRequestHandlerMemeCreate mock_hdlr(empty_config);
    int id1 = 0;
    int id2 = 0;
    MemeEntry entry("image_1", "top_sim", "top_2", 0);
    std::thread t1(&MockRequestHandlerMemeCreate::insertToStorage, &mock_hdlr, std::ref(entry), std::ref(id1));
    std::thread t2(&MockRequestHandlerMemeCreate::insertToStorage, &mock_hdlr, std::ref(entry), std::ref(id2));
    t1.join();
    t2.join();
    std::cout<<"t1: " << id1 << " t2: " << id2 <<std::endl;
    EXPECT_TRUE(id1 != id2);
}

TEST(RequstHandlerMemeCreateTest, ConcurrencyTest2) {
    NginxConfig empty_config;
    MockRequestHandlerMemeCreate mock_hdlr(empty_config);
    std::vector<std::thread> threads;
    std::vector<int> ids;
    std::vector<MemeEntry> entries;
    for (int i = 0; i < 10; i++) {
        ids.push_back(0);
        entries.emplace_back(MemeEntry("image_1", "top_1_"+std::to_string(i), "bottom_1_"+std::to_string(i), 0));
    }
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(std::thread(&MockRequestHandlerMemeCreate::insertToStorage, &mock_hdlr, std::ref(entries[i]), std::ref(ids[i])));
    }
    for (int i = 0; i < 10; i++) threads[i].join();
    for (int i = 0; i < 10; i++) std::cout << "id: " << ids[i] << std::endl;
    std::sort(ids.begin(), ids.end());
    auto it = std::unique(ids.begin(), ids.end());
    bool isUnique = (it == ids.end());
    EXPECT_TRUE(isUnique);
}