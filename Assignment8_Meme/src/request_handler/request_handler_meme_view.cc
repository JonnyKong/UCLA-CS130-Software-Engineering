#include <iostream>

#include "request_handler/request_handler_meme_view.h"
#include "session.h"

RequestHandlerMemeView::RequestHandlerMemeView(const NginxConfig &config) : database_name("../assets/meme.db") {
    name2uri["Tom"] = "http://127.0.0.1:8080/static/image1.jpg";
    name2uri["simply.jpg"] = "https://upload.wikimedia.org/wikipedia/commons/thumb/8/8e/Rubber_duckies_So_many_ducks.jpg/1280px-Rubber_duckies_So_many_ducks.jpg";
    name2uri["grumpy.jpg"] = "https://upload.wikimedia.org/wikipedia/commons/thumb/e/ee/Grumpy_Cat_by_Gage_Skidmore.jpg/800px-Grumpy_Cat_by_Gage_Skidmore.jpg";
}

int RequestHandlerMemeView::sqlCallback(void*data, int argc, char**argv, char**azColName) {
    std::vector<std::string> * sql_str = (std::vector<std::string> *) data;
    std::cout<<"argc: "<<argc<<std::endl;
    if (argc == 0) return -1;
    for (int i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        sql_str->push_back(argv[i]);    
    }
    return 0;
};

std::unique_ptr<reply> RequestHandlerMemeView::handleRequest(const request & request_) noexcept {
   //parse the uri
    std::string id_str = request_.uri.substr(request_.uri.find_last_of("/")+1);
    //id check
    if(id_str.size() == 0 ) {
        std::cout<<"original uri"<<std::endl;
        std::cout<<"invalid meme id"<<std::endl;
    }
    for (int i = 0; i < id_str.length(); i++) {
        if (!std::isdigit(id_str[i])) {
            std::cout<<"invalid meme id"<<std::endl;
            break;
        }
    }

    //======Database Part========
    sqlite3 *db;
    char *err_message = 0;
    int rc;
    char * sql;
    std::vector<std::string> fetched_tuple;

    //open database
    rc = sqlite3_open(database_name.c_str(), &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    //find the record
    std::string sql_resp;
    std::string query_str = "SELECT * FROM tbl1 WHERE oid="+id_str;
    const char *sql_query= query_str.c_str();
    std::string display_html_content;
    rc = sqlite3_exec(db, sql_query, sqlCallback, (void*)(&fetched_tuple), &err_message);
    if (fetched_tuple.size() == 0) {
        sql_resp = "MEME ID does Not Exist\n";
        sqlite3_free(err_message);
        sqlite3_close(db);
        // exit(1);
        display_html_content = "This meme id does not exist";
    } else { 
        sql_resp = "Find the record!";
        sqlite3_free(err_message);
        sqlite3_close(db);
        display_html_content = fetchImage(name2uri[fetched_tuple[0]],fetched_tuple[1], fetched_tuple[2]);
    }
    std::cout<<sql_resp<<std::endl;
    //========end of sql=======
    int meme_idx = std::stoi(id_str);
    std::cout<<"meme id: "<<meme_idx<<std::endl;
    std::cout<<request_.uri<<std::endl;
    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    reply_->status = reply::ok;
    reply_->headers.resize(2);
    reply_->content = display_html_content;
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/html";
    return reply_;
};

std::string RequestHandlerMemeView::fetchImage(const std::string& img_uri, const std::string& top_txt, const std::string& bottom_txt) {
    // return "An html page will be displayed";
    std::string display_html_content = "<html>\n"
    "<style>\n"
        "body { display: inline-block; position: relative; }\n"
        "span { color: white; font: 2em bold Impact, sans-serif; position: absolute; text-align: center; width: 100%; }\n"
        "#top { top: 0; color: blue;}\n"
        "#bottom { bottom: 0; color: blue;}\n"
    "</style>\n"
    "<body>\n"                                                                                                                                         
        "<img src=\""+img_uri+"\">\n"
        "<span id=\"top\">"+top_txt+"</span>\n"
        "<span id=\"bottom\">"+bottom_txt+"</span>\n"
    "</body>\n"
    "</html>\n";
    return display_html_content;
}
