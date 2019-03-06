#include <iostream>
#include <cassert>
#include <string>
#include <mutex>
#include <thread>
#include "request_handler/request_handler_meme_create.h"
#include "session.h"

/**
 * Constructor: Maybe init the database and tables. The constructor doesn't need 
 *  to take config as input.
 */
RequestHandlerMemeCreate::RequestHandlerMemeCreate(const NginxConfig &config) 
  : database_name("../assets/meme.db")
{
  maybeInit(database_name);
}


/**
 * handleRequest() - Parse the request, and write to persistent store. Then, 
 *  return a HTTP response as to whether the operation has succeeded.
 */
std::unique_ptr<reply> RequestHandlerMemeCreate::handleRequest(const request &request_) noexcept {
  std::unique_ptr<reply> reply_ = std::make_unique<reply>();
  std::cout << "RequestHandlerMemeCreate::handleRequest()" << std::endl;

  std::map<std::string, std::string> params = parseRESTParams(request_.uri);

  // If no image parameter in params, return bad request  
  auto it = params.find("image");
  if (it == params.end() || it -> second.empty()) {
    reply_ = http::server::reply::stock_reply(reply::bad_request);
    return reply_;
  }

  // If no top/bottom in params, set to empty
  if (params.find("top") == params.end())
    params["top"] = "";
  if (params.find("bottom") == params.end())
    params["bottom"] = "";
  
  MemeEntry entry(params["image"], params["top"], params["bottom"]);
  std::replace(entry.top.begin(), entry.top.end(), '+', ' ');
  std::replace(entry.bottom.begin(), entry.bottom.end(), '+', ' ');
  int id;
  std::string ret = insertToStorage(entry, id);
  if (ret != "SUCCESS")
    reply_ = http::server::reply::stock_reply(reply::bad_request);
  else {

    std::string display_html_content = "<html>\n"
                               "<title>Created meme!</title>\n"
                               "<body> Meme Created! <a href=\"/meme/view/"+std::to_string(id)+"\">"+"Meme ID: " + std::to_string(id)+"</a>"
                               "</body>\n"
                               "</html>\n";
    reply_->status = reply::ok;
    reply_->headers.resize(2);
    reply_->content = display_html_content;
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/html";
  }
  return reply_;
}

std::mutex RequestHandlerMemeCreate::mtx;

/**
 * insertToStorage() - Insert this entry into persistent storage
 */
std::string RequestHandlerMemeCreate::insertToStorage(const MemeEntry &entry, int &id) {
  std::lock_guard<std::mutex> lock(mtx);
  std::cout<<"start "<< std::this_thread::get_id()<< std::endl;
  sqlite3 *db;
  char *err_message = 0;
  int rc;
  char *sql;
  sqlite3_stmt *stmt;
  std::string ret;
  // Open database
  rc = sqlite3_open(database_name.c_str(), &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(1);
  }

  // Insert record
  const char *sql_query = "INSERT INTO tbl1 VALUES("
                          "?1, "
                          "?2, "
                          "?3  "
                          ")";
  sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, entry.image.c_str(),  -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, entry.top.c_str(),    -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, entry.bottom.c_str(), -1, SQLITE_STATIC);
  std::cout<<"bottom txt: "<<entry.bottom<<std::endl;
  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rc != SQLITE_DONE) {
    // ret = fmt::sprintf("ERROR inserting data: %s\n", sqlite3_errmsg(db));
    ret = std::string("ERROR inserting data: ") + 
          std::string(sqlite3_errmsg(db)) + 
          "\n";
    std::cout << ret << std::endl;
    id = -1;
  } else {
    ret = "SUCCESS";
    id = getTableSize();
  }
  sqlite3_close(db);
  std::cout<<"end "<< std::this_thread::get_id()<< std::endl;
  return ret;
}

int RequestHandlerMemeCreate::sqlCount(void*data, int argc, char**argv, char**azColName) {
    int * tbl_cnt = (int *) data;
    if (argc == 0) return -1;
    for (int i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        *tbl_cnt = std::atoi(argv[i]);
        return 1;  
    }
    return 1;
};

// get table size
int RequestHandlerMemeCreate::getTableSize() noexcept {
    //======Database Part========
    sqlite3 *db;
    char *err_message = 0;
    int rc;
    char * sql;
    int tbl_cnt;
    //open database
    rc = sqlite3_open(database_name.c_str(), &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    //find the record
    std::string sql_resp;
    std::string query_str = "SELECT COUNT(*) FROM tbl1";
    const char *sql_query= query_str.c_str();
    rc = sqlite3_exec(db, sql_query, sqlCount, (void*)(&tbl_cnt), &err_message);
    std::cout<<"table count: "<<tbl_cnt<<std::endl;
    sqlite3_close(db);
    return tbl_cnt;
};



/**
 * parseRESTParams() - Given an RESTful URI, return a map of its params.
 */
std::map<std::string, std::string> RequestHandlerMemeCreate::
parseRESTParams(const std::string &uri) {
  size_t cursor = uri.find("?") + 1;
  std::map<std::string, std::string> params;
  while (cursor < uri.size() && cursor != std::string::npos) {
    size_t cursor_equal = uri.find("=", cursor);
    size_t cursor_next_param = uri.find("&", cursor);
    std::string param = uri.substr(cursor, cursor_equal - cursor);
    std::string value;
    if (cursor_equal != std::string::npos && cursor_equal < cursor_next_param) {
      if (cursor_next_param != std::string::npos)
        value = uri.substr(cursor_equal + 1, cursor_next_param - cursor_equal - 1);
      else
        value = uri.substr(cursor_equal + 1, uri.length() - cursor_equal - 1);
    } else {
      value = "";
    }           
    params[param] = value;   
    cursor = (cursor_next_param == std::string::npos) ? 
             std::string::npos : cursor_next_param + 1;
  }
  return params;
}

// TODO: Return success status to user?