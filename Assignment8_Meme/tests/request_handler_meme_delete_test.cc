#include <iostream>

#include "gtest/gtest.h"
#include "http/request.h"
#include "http/request_parser.h"
#include "config_parser.h"
#include "http/reply.h"
#include "request_handler_dispatcher.h"  // Include typedefs
#include "http/mime_types.h"
#include "request_handler/request_handler_meme_create.h"
#include "request_handler/request_handler_meme_delete.h"
#include "session.h"
#include <string>

class MemeViewTest : public :: testing::Test{
protected:
  NginxConfig config;
};

class MockRequestHandlerMemeCreate : public RequestHandlerMemeCreate {
public:
    MockRequestHandlerMemeCreate(const NginxConfig &config) {
        database_name = std::string("../assets/meme_test.db");
        remove(database_name.c_str());  // Doesn't matter if failed
        maybeInit(database_name);
    }

    int getTableSize() {
        sqlite3 *db;
        int rc;
        char * sql;
        sqlite3_stmt *stmt;
        int cnt;

        //open database
        rc = sqlite3_open(database_name.c_str(), &db);
        std::string query_str = "SELECT Count(*) FROM tbl1";
        const char *sql_query= query_str.c_str();
        sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
        sqlite3_step(stmt);
        cnt = sqlite3_column_int(stmt, 0);
        rc = sqlite3_clear_bindings(stmt);
        rc = sqlite3_reset(stmt);
        std::string display_html_content;
        sqlite3_close(db);
        return cnt;
    }
    ~MockRequestHandlerMemeCreate() {
        remove(database_name.c_str());
    }
};

class MockRequestHandlerMemeDelete : public RequestHandlerMemeDelete {
public:
    MockRequestHandlerMemeDelete(const NginxConfig &config) {
        database_name = std::string("../assets/meme_test.db");
        remove(database_name.c_str());  // Doesn't matter if failed
        maybeInit(database_name);
    }
    ~MockRequestHandlerMemeDelete() {
        remove(database_name.c_str());
    }
};


TEST(RequestHandlerMemeDeleteTest, DeleteTest) {
    NginxConfig empty_config;
    MockRequestHandlerMemeCreate mock_hdlr(empty_config);
    MockRequestHandlerMemeDelete delete_hdlr(empty_config);
    http::server::request req;
    req.uri = "/meme/view?id=1";
    int id;
    MemeEntry entry("simply.jpg", "top_text", "bottom_text", 0);
    std::string result = mock_hdlr.insertToStorage(entry, id);
    EXPECT_TRUE(result == std::string("SUCCESS"));
    EXPECT_TRUE(mock_hdlr.getTableSize() == 1);
    delete_hdlr.handleRequest(req);
    EXPECT_TRUE(mock_hdlr.getTableSize() == 0);
}

TEST(RequestHandlerMemeDeleteTest, DeleteTestEmpty) {
    NginxConfig empty_config;
    MockRequestHandlerMemeCreate mock_hdlr(empty_config);
    MockRequestHandlerMemeDelete delete_hdlr(empty_config);
    http::server::request req;
    req.uri = "/meme/view?id=1";
    delete_hdlr.handleRequest(req);
    EXPECT_TRUE(mock_hdlr.getTableSize() == 0);
}
