#include <iostream>
#include <cassert>

#include "request_handler/request_handler_meme_list.h"
#include "session.h"

RequestHandlerMemeList::RequestHandlerMemeList(const NginxConfig &config) 
  : database_name("../assets/meme.db")
{
  maybeInit(database_name);
}

std::unique_ptr<reply> RequestHandlerMemeList::handleRequest(const request &request_) noexcept {
    //TODO(Yijun): Change this to log
    std::cout << "RequestHandlerMemeCreate::handleRequest()" << std::endl;
    std::string search_content = parseSearch(request_.uri);
    std::cout << "search_content: " << search_content << std::endl;
    std::cout << search_content.size() << std::endl;

    std::vector<MemeEntry> meme_list;
    if (search_content.size() == 0) {
        meme_list = RequestHandlerMemeList::selectAllMeme();
    } else {
        // do select with substring match
        std::cout << "Searching for: " << search_content << std::endl;
        meme_list = RequestHandlerMemeList::searchMeme(search_content);
    }
    
    std::string body;
    for(int i = 0; i < meme_list.size(); i++) {
        // std::cout << "Image: " << meme_list[i].image << std::endl;
        // std::cout << "Top: " << meme_list[i].top << std::endl;
        // std::cout << "Bottom: " << meme_list[i].bottom << std::endl;
        // std::cout << "ID: " << meme_list[i].id << std::endl;
        int id = meme_list[i].id;
        body += boost::str(boost::format("<a href= \"/meme/view?id=%s\">Meme ID: %s</a><br>")% id % id);
        // std::cout << "Body: " << body << std::endl;
    }
    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    reply_->status = reply::ok;    // 200
    reply_->headers.resize(2);
    reply_->content = "<html><body>" + body + "</body></html>";
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/html";
    std::cout << "Body: " << reply_->content << std::endl;
	return reply_;
}

std::vector<MemeEntry> RequestHandlerMemeList::selectAllMeme(){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *sql;
    const char* data = "Callback function called";

	std::vector<MemeEntry> meme_list;

    rc = sqlite3_open(database_name.c_str(), &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    std::string sql_str =  "SELECT * from tbl1";
	sql = sql_str.c_str();

	rc = sqlite3_exec(db, sql, RequestHandlerMemeList::appendMeme, (void*)&meme_list, &zErrMsg);

	if( rc != SQLITE_OK ) {
      	fprintf(stderr, "SQL error: %s\n", zErrMsg);
      	sqlite3_free(zErrMsg);
   	} else {
      	fprintf(stdout, "Operation done successfully\n");
   	}
   	sqlite3_close(db);
   	return meme_list;

}

int RequestHandlerMemeList::appendMeme(void *data, int argc, char **argv, char **azColName){
	std::vector<MemeEntry> *p = static_cast<std::vector<MemeEntry>*>(data);
	p->push_back(MemeEntry(std::string(argv[1]), std::string(argv[2]), std::string(argv[3]), atoi(argv[0])));
	return 0;
}

std::vector<MemeEntry> RequestHandlerMemeList::searchMeme(std::string &search_content){
    sqlite3 *db;
    int rc;
    char * sql;
    sqlite3_stmt *stmt;
    std::vector<MemeEntry> meme_list;

    //open database
    rc = sqlite3_open(database_name.c_str(), &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    //find the record
    std::string sql_resp;
    std::string query_str = "SELECT * FROM tbl1 WHERE image LIKE ?1 OR top LIKE ?1 OR bottom LIKE ?1";
    const char *sql_query= query_str.c_str();
    sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, search_content.c_str(), -1, SQLITE_STATIC);
    std::cout << "Statement Preparation Completed!" << std::endl;
    while ( sqlite3_step( stmt ) == SQLITE_ROW ) {
            meme_list.push_back(
                MemeEntry(std::string(reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, 1))),
                    std::string(reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, 2))),
                    std::string(reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, 3))),
                    atoi(reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, 0)))
                    )
            );
    }
    std::cout << "Query Selection Completed!" << std::endl;
    rc = sqlite3_clear_bindings(stmt);
    rc = sqlite3_reset(stmt);
    if (meme_list.size() == 0) {
        sql_resp = "No Matching\n";
        sqlite3_close(db);
    } else { 
        sql_resp = "Find the record!";
        sqlite3_close(db);
    }
    std::cout << sql_resp << std::endl;
    return meme_list;
}

std::string RequestHandlerMemeList::parseSearch (const std::string &uri) {
    size_t cursor = uri.find("=");
    if (cursor == std::string::npos) {
        return std::string("");
    }
    return uri.substr(cursor + 1);
}
