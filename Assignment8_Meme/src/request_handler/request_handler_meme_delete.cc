#include <iostream>
#include <string>
#include "request_handler/request_handler_meme_delete.h"
#include "session.h"
#include "request_handler/meme_common.h"

RequestHandlerMemeDelete::RequestHandlerMemeDelete(const NginxConfig &config) : database_name("../assets/meme.db") {

};

std::unique_ptr<reply> RequestHandlerMemeDelete::handleRequest(const request & request_) noexcept {
    //parse the uri
    std::map<std::string, std::string> params = parseRESTParams(request_.uri);
    std::string id_str = params["id"];
    std::cout<<"meme id: "<<id_str<<std::endl;
    //id check
    if(id_str.size() == 0 ) {
        std::cout<<"original uri"<<std::endl;
        std::cout<<"invalid meme id"<<std::endl;
        exit(1);
    }
    for (int i = 0; i < id_str.length(); i++) {
        if (!std::isdigit(id_str[i])) {
            std::cout<<"invalid meme id"<<std::endl;
            exit(1);
            break;
        }
    }
    int meme_idx = std::stoi(id_str);
    //======Database Part========
    sqlite3 *db;
    int rc;
    char * sql;
    sqlite3_stmt *stmt;
    std::vector<std::string> fetched_tuple;
    std::string ret;
    //open database
    rc = sqlite3_open(database_name.c_str(), &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    //delete the record
    const char *sql_query = "DELETE FROM tbl1 WHERE id=?1";
    sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, meme_idx);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        ret = "No Such Element";
    } else {
        ret = "deleted the element";
    }
    rc = sqlite3_clear_bindings(stmt);
    rc = sqlite3_reset(stmt);
    //========end of sql=======
    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    reply_->status = reply::ok;
    reply_->headers.resize(2);
    reply_->content = ret;
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/html";
    return reply_;
};
