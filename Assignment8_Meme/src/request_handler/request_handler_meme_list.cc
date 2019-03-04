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

    std::vector<MemeEntry> meme_list = RequestHandlerMemeList::selectAllMeme();
    std::string body;
    for(int i = 0; i < meme_list.size(); i++) {
        // std::cout << "Image: " << meme_list[i].image << std::endl;
        // std::cout << "Top: " << meme_list[i].top << std::endl;
        // std::cout << "Bottom: " << meme_list[i].bottom << std::endl;
        // std::cout << "ID: " << i << std::endl;
        body += boost::str(boost::format("<a href= \"/meme/view/%s\">Meme ID: %s</a><br>")% (i+1) % (i+1));
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
	p->push_back(MemeEntry(std::string(argv[0]), std::string(argv[1]), std::string(argv[2])));
	return 0;
}
