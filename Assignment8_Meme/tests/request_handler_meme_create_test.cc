#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>

#include "config_parser.h"
#include "http/mime_types.h"
#include "http/reply.h"
#include "http/request.h"
#include "http/request_parser.h"
#include "request_handler/request_handler_meme_create.h"
#include "request_handler/request_handler_meme_form.h"
#include "request_handler/request_handler_meme_list.h"
#include "request_handler/request_handler_meme_view.h"
#include "request_handler_dispatcher.h" // Include typedefs
#include "gtest/gtest.h"

/**
 * MockRequestHandlerMemeCreate() - Wrapper class to avoid creating the default
 *  database file. Instead, create a test database, and remove it when finish
 *  running the unit tests.
 */
class MockRequestHandlerMemeCreate : public RequestHandlerMemeCreate {
public:
  MockRequestHandlerMemeCreate(const NginxConfig &config) {
    database_name = std::string("../assets/meme_test.db");
    remove(database_name.c_str()); // Doesn't matter if failed
    maybeInit(database_name);
  }
  ~MockRequestHandlerMemeCreate() { remove(database_name.c_str()); }
};

// Should success: Insert one entry into the database
TEST(RequestHandlerMemeCreateTest, InsertionTest) {
  NginxConfig empty_config;
  MockRequestHandlerMemeCreate mock_hdlr(empty_config);

  int id;
  MemeEntry entry("image_1", "top_1", "top_2", 0);
  std::string result = mock_hdlr.insertToStorage(entry, id);

  EXPECT_TRUE(result == "SUCCESS");
}

// Should fail: Insert duplicate entries into the database
TEST(RequestHandlerMemeCreateTest, DuplicateInsertionTest) {
  NginxConfig empty_config;
  MockRequestHandlerMemeCreate mock_hdlr(empty_config);
  int id;
  MemeEntry entry("image_1", "top_1", "top_2", 0);

  std::string result_1 = mock_hdlr.insertToStorage(entry, id);
  std::string result_2 = mock_hdlr.insertToStorage(entry, id);

  EXPECT_TRUE(result_1 == "SUCCESS");
  EXPECT_TRUE(result_2 != "SUCCESS");
}

// Should success: Insert and update
TEST(RequestHandlerMemeCreateTest, UpdateTest) {
  NginxConfig empty_config;
  MockRequestHandlerMemeCreate mock_hdlr(empty_config);
  int id;

  MemeEntry entry_insert("image_1", "top_1", "top_2", 0);
  std::string result_1 = mock_hdlr.insertToStorage(entry_insert, id);
  MemeEntry entry_update("image_1_update", "top_1_update", "top_2_update", id);
  std::string result_2 = mock_hdlr.updateStorage(entry_update);

  EXPECT_TRUE(result_1 == "SUCCESS");
  EXPECT_TRUE(result_2 == "SUCCESS");
}

// Should success: Even if the entry doesn't exist in database, updating still
//  returns success. This test is here in case sqlite3 implementation changes
//  in the future.
TEST(RequestHandlerMemeCreateTest, UpdateNullTest) {
  NginxConfig empty_config;
  MockRequestHandlerMemeCreate mock_hdlr(empty_config);
  int id = 42;

  MemeEntry entry_update("image_1_update", "top_1_update", "top_2_update", id);
  std::string result = mock_hdlr.updateStorage(entry_update);

  EXPECT_TRUE(result == "SUCCESS");
}

// Should success: Get max id in sql table
TEST(RequestHandlerMemeCreateTest, MaxIdTest) {
  NginxConfig empty_config;
  MockRequestHandlerMemeCreate mock_hdlr(empty_config);

  int id;
  mock_hdlr.insertToStorage(MemeEntry("image_1", "top_1", "top_1"), id);
  mock_hdlr.insertToStorage(MemeEntry("image_2", "top_2", "top_2"), id);
  int max_id = mock_hdlr.getMaxId();

  EXPECT_TRUE(max_id == id);
}

// Should success: Parse REST params
TEST(RequestHandlerMemeCreateTest, ParseRESTParamsTest) {
  NginxConfig empty_config;
  MockRequestHandlerMemeCreate mock_hdlr(empty_config);

  std::string uri = "foo/bar?param1=value1&param2=value2";
  std::map<std::string, std::string> params = parseRESTParams(uri);

  EXPECT_TRUE(params.size() == 2);
  EXPECT_TRUE(params["param1"] == "value1");
  EXPECT_TRUE(params["param2"] == "value2");
}

// Should success: Parse REST with encoded URL
TEST(RequestHandlerMemeCreateTest, ParseRESTParamsDecodingTest) {
  NginxConfig empty_config;
  MockRequestHandlerMemeCreate mock_hdlr(empty_config);

  std::string uri = "foo/bar?param1=foo+bar&param2=\%21\%22\%26\%28";
  std::map<std::string, std::string> params = parseRESTParams(uri);

  EXPECT_TRUE(params.size() == 2);
  EXPECT_TRUE(params["param1"] == "foo bar");
  EXPECT_TRUE(params["param2"] == "!\"&(");
}

// Should success: Test escape() function that escapes user input that are
//  html-sensitive
TEST(RequestHandlerMemeCreateTest, ParseRESTParamsEscapingTest) {
  std::string user_input = "funny text \" onclick=\"location='http://evil.com'";
  std::string escaped = escape(user_input);
  std::string expected =
      "funny text &quot; onclick=&quot;location=&apos;http://evil.com&apos;";

  EXPECT_TRUE(escaped == expected);
}

// Should success: Concurrently insertion into databaes
TEST(RequstHandlerMemeCreateTest, ConcurrencyTest1) {
  NginxConfig empty_config;
  MockRequestHandlerMemeCreate mock_hdlr(empty_config);
  int id1 = 0;
  int id2 = 0;
  MemeEntry entry("image_1", "top_sim", "top_2", 0);
  std::thread t1(&MockRequestHandlerMemeCreate::insertToStorage, &mock_hdlr,
                 std::ref(entry), std::ref(id1));
  std::thread t2(&MockRequestHandlerMemeCreate::insertToStorage, &mock_hdlr,
                 std::ref(entry), std::ref(id2));
  t1.join();
  t2.join();

  EXPECT_TRUE(id1 != id2);
}

// Should success: Concurrently insertion into databaes (10 threads)
TEST(RequstHandlerMemeCreateTest, ConcurrencyTest2) {
  NginxConfig empty_config;
  MockRequestHandlerMemeCreate mock_hdlr(empty_config);
  std::vector<std::thread> threads;
  std::vector<int> ids;
  std::vector<MemeEntry> entries;
  for (int i = 0; i < 10; i++) {
    ids.push_back(0);
    entries.emplace_back(MemeEntry("image_1", "top_1_" + std::to_string(i),
                                   "bottom_1_" + std::to_string(i), 0));
  }
  for (int i = 0; i < 10; i++) {
    threads.emplace_back(
        std::thread(&MockRequestHandlerMemeCreate::insertToStorage, &mock_hdlr,
                    std::ref(entries[i]), std::ref(ids[i])));
  }
  for (int i = 0; i < 10; i++)
    threads[i].join();
    
  std::sort(ids.begin(), ids.end());
  auto it = std::unique(ids.begin(), ids.end());
  bool isUnique = (it == ids.end());

  EXPECT_TRUE(isUnique);
}
