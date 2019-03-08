#include <iostream>

#include "gtest/gtest.h"
#include "request_handler/request_handler_meme_view.h"
#include "http/request.h"
#include "http/request_parser.h"
#include "config_parser.h"
#include "http/reply.h"
#include "request_handler_dispatcher.h"  // Include typedefs
#include "http/mime_types.h"
#include "request_handler/request_handler_meme_create.h"
#include "session.h"

class MemeViewTest : public :: testing::Test{
protected:
  NginxConfig config;
};


//test whether the generated html page is correct
TEST_F(MemeViewTest, TestItsHTMLFunction) {
    RequestHandlerMemeView meme_view(config);
    std::string img_uri = "https://upload.wikimedia.org/wikipedia/commons/thumb/8/8e/Rubber_duckies_So_many_ducks.jpg/1280px-Rubber_duckies_So_many_ducks.jpg";
    std::string top_txt = "we are";
    std::string bottom_txt = "rubber duck";
    std::string expected_html = "<html>\n"
    "<style>\n"
        "body { display: inline-block; position: relative; }\n"
        "span { color: white; font: 2em bold Impact, sans-serif; position: absolute; text-align: center; width: 100%; }\n"
        "#top { top: 0; color: blue;}\n"
        "#bottom { bottom: 0; color: blue;}\n"
    "</style>\n"
    "<body>\n"                                                                                                                                         
        "<img src=\""+img_uri+"\">\n"
        "<br/>\n"
        "<span id=\"top\">"+top_txt+"</span>\n"
        "<span id=\"bottom\">"+bottom_txt+"</span>\n"
        "<span>\n"
        "<a href=\"/meme/delete?id=1\" onclick=\"return confirm('Are you sure?')\">Delete</a>\n"
        "<a href=\"/meme/new?update=1\">Edit</a>\n"
        "<span\n>\n"
    "</body>\n"
    "</html>\n";
    std::string test_html = meme_view.fetchImage("1", img_uri, top_txt, bottom_txt);
    std::cout << test_html << std::endl;
    EXPECT_TRUE(test_html == expected_html);
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

class MockRequestHandlerMemeView : public RequestHandlerMemeView {
public:
    MockRequestHandlerMemeView(const NginxConfig &config, const std::string & db_name) {
        database_name = db_name;
        name2uri["simply.jpg"] = "https://upload.wikimedia.org/wikipedia/commons/thumb/8/8e/Rubber_duckies_So_many_ducks.jpg/1280px-Rubber_duckies_So_many_ducks.jpg";
    }
};

TEST(RequestHandlerMemeViewTest, viewTest) {
    NginxConfig empty_config;
    MockRequestHandlerMemeCreate mock_hdlr(empty_config);
    http::server::request req;
    req.uri = "/meme/view?id=1";
    int id;
    MemeEntry entry("simply.jpg", "top_text", "bottom_text", 0);
    std::string result = mock_hdlr.insertToStorage(entry, id);
    EXPECT_TRUE(result == std::string("SUCCESS"));
    std::string db_name = "../assets/meme_test.db";
    MockRequestHandlerMemeView mock_view_hdlr(empty_config, db_name);
    std::unique_ptr<reply> rep = mock_view_hdlr.handleRequest(req);
    std::string test_content = rep->content;
    std::cout << test_content << std::endl;
    std::string expected_content = "<html>\n"
    "<style>\n"
        "body { display: inline-block; position: relative; }\n"
        "span { color: white; font: 2em bold Impact, sans-serif; position: absolute; text-align: center; width: 100%; }\n"
        "#top { top: 0; color: blue;}\n"
        "#bottom { bottom: 0; color: blue;}\n"
    "</style>\n"
    "<body>\n"                                                                                                                                         
        "<img src=\"https://upload.wikimedia.org/wikipedia/commons/thumb/8/8e/Rubber_duckies_So_many_ducks.jpg/1280px-Rubber_duckies_So_many_ducks.jpg\">\n"
        "<br/>\n"
        "<span id=\"top\">top_text</span>\n"
        "<span id=\"bottom\">bottom_text</span>\n"
        "<span>\n"
        "<a href=\"/meme/delete?id=1\" onclick=\"return confirm('Are you sure?')\">Delete</a>\n"
        "<a href=\"/meme/new?update=1\">Edit</a>\n"
        "<span\n>\n"
    "</body>\n"
    "</html>\n";
    EXPECT_TRUE(test_content == expected_content);
}