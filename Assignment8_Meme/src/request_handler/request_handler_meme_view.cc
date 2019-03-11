#include <iostream>
#include <string>
#include "request_handler/request_handler_meme_view.h"
#include "session.h"
#include "request_handler/meme_common.h"

RequestHandlerMemeView::RequestHandlerMemeView(const NginxConfig &config) : database_name("../assets/meme.db") {
    name2uri["Tom"] = "https://upload.wikimedia.org/wikipedia/en/f/f6/Tom_Tom_and_Jerry.png";
    name2uri["simply.jpg"] = "https://upload.wikimedia.org/wikipedia/commons/thumb/8/8e/Rubber_duckies_So_many_ducks.jpg/1280px-Rubber_duckies_So_many_ducks.jpg";
    name2uri["grumpy.jpg"] = "https://upload.wikimedia.org/wikipedia/commons/thumb/e/ee/Grumpy_Cat_by_Gage_Skidmore.jpg/800px-Grumpy_Cat_by_Gage_Skidmore.jpg";
    name2uri["rubberduck"] = "https://i.gifer.com/FyBy.gif";
}

int RequestHandlerMemeView::sqlCallback(void*data, int argc, char**argv, char**azColName) {
    std::vector<std::string> * sql_str = (std::vector<std::string> *) data;;
    if (argc == 0) return -1;
    for (int i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        sql_str->push_back(argv[i]);
    }
    return 0;
};

std::unique_ptr<reply> RequestHandlerMemeView::handleRequest(const request & request_) noexcept {
   //parse the uri
    std::map<std::string, std::string> params = parseRESTParams(request_.uri);
    std::string id_str = params["id"];
    //id check
    if(id_str.size() == 0 ) {
        return generateReply("invalid meme id", reply::bad_request);
    }
    for (int i = 0; i < id_str.length(); i++) {
        if (!std::isdigit(id_str[i])) {
            return generateReply("invalid meme id", reply::bad_request);
        }
    }
    int meme_idx = std::stoi(id_str);
    //======Database Part========
    sqlite3 *db;
    int rc;
    char * sql;
    sqlite3_stmt *stmt;
    std::vector<std::string> fetched_tuple;

    //open database
    rc = sqlite3_open(database_name.c_str(), &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return generateReply("Cannot open database", reply::service_unavailable);
    }
    //find the record
    std::string sql_resp;
    std::string query_str = "SELECT * FROM tbl1 WHERE id=?1";
    const char *sql_query= query_str.c_str();
    sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, meme_idx);
    while ( sqlite3_step( stmt ) == SQLITE_ROW ) {
        for ( int colIndex = 0; colIndex < sqlite3_column_count(stmt); colIndex++ ) {
            fetched_tuple.push_back(
                std::string(reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, colIndex))
                )
            );
        }
    }
    rc = sqlite3_clear_bindings(stmt);
    rc = sqlite3_reset(stmt);
    std::string display_html_content;
    if (fetched_tuple.size() == 0) {
        sqlite3_close(db);
        display_html_content = "This meme id does not exist";
        return generateReply(display_html_content, reply::not_found);
    } else {
        sqlite3_close(db);
        display_html_content = fetchImage(fetched_tuple[0],
                                          name2uri[fetched_tuple[1]],
                                          fetched_tuple[2],
                                          fetched_tuple[3]);
        return generateReply(display_html_content, reply::ok);
    }
    //========end of sql=======
};

std::unique_ptr<reply> RequestHandlerMemeView::generateReply(const std::string &html_content, http::server::reply::status_type status) {
    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    reply_->status = status;
    reply_->headers.resize(2);
    reply_->content = html_content;
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/html";
    return reply_;
}

/**
 * fetchImage() - Generate image using given image name and meme text. Id is
 *  used to generate link for edit.
 */
std::string RequestHandlerMemeView::fetchImage(const std::string &id,
                                               const std::string &img_uri,
                                               const std::string &top_txt,
                                               const std::string &bottom_txt) {
    // return "An html page will be displayed";
    std::string display_html_content = "<html>\n"
    "<style>\n"
        "body { display: inline-block; position: relative; }\n"
        "span { color: white; font: 2em bold Impact, sans-serif; position: absolute; text-align: center; width: 100%; }\n"
        "#top { top: 0; color: blue;}\n"
        "#bottom { bottom: 0; color: blue;}\n"
    "</style>\n"
    "<body>\n"
        "<img src=\"" + img_uri + "\">\n"
        "<br/>\n"
        "<span id=\"top\">" + top_txt + "</span>\n"
        "<span id=\"bottom\">" + bottom_txt + "</span>\n"
        "<span>\n"
        "<a href=\"/meme/delete?id=" + id + "\" onclick=\"return confirm('Are you sure?')\">Delete</a>\n"
        "<a href=\"/meme/new?update=" + id + "\">Edit</a>\n"
        "<span\n>\n"
    "</body>\n"
    "</html>\n";
    return display_html_content;
}
