#include <iostream>
#include <cassert>

#include "request_handler/request_handler_meme_create.h"
#include "session.h"

/**
 * Constructor: Maybe init the database and tables. The constructor doesn't need 
 *  to take config as input.
 */
RequestHandlerMemeCreate::RequestHandlerMemeCreate(const NginxConfig &config) 
  : database_name("../assets/meme.db")
{
  maybeInit();
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
  
  MemeEntry entry(params["string"], params["top"], params["bottom"]);
  std::string ret = insertToStorage(entry);
  if (ret != "SUCCESS")
    reply_ = http::server::reply::stock_reply(reply::bad_request);
  else
    reply_ = http::server::reply::stock_reply(reply::created);
  return reply_;
}




/**
 * createTableIfNotExists() - When the server is deployed to a new server, need
 *  to create the database and table. This function is called by the constructor.
 */
void RequestHandlerMemeCreate::maybeInit() {
  sqlite3 *db;
  char *err_message = 0;
  int rc;
  char *sql;
  sqlite3_stmt *res;

  // Open database
  rc = sqlite3_open(database_name.c_str(), &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(1);
  }

  // Create table if not exists. Use all columns as primary key to prevent
  //  inserting duplicate entries into the table.
  const char *sql_query = "CREATE TABLE IF NOT EXISTS tbl1 ("
                          "image  varchar(100), "
                          "top    varchar(100), "
                          "bottom varchar(100), "
                          "PRIMARY KEY (image, top, bottom)"
                          ")";
  rc = sqlite3_exec(db, sql_query, 0, 0, &err_message);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_message);
    sqlite3_free(err_message);
    sqlite3_close(db);
    exit(1);
  } else {
    fprintf(stdout, "Table tbl1 created successfully or already exists\n");
  }

  sqlite3_close(db);
}




/**
 * insertToStorage() - Insert this entry into persistent storage
 */
std::string RequestHandlerMemeCreate::insertToStorage(const MemeEntry &entry) {
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
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    // ret = fmt::sprintf("ERROR inserting data: %s\n", sqlite3_errmsg(db));
    ret = std::string("ERROR inserting data: ") + 
          std::string(sqlite3_errmsg(db)) + 
          "\n";
    std::cout << ret << std::endl;
  } else {
    ret = "SUCCESS";
  }
  sqlite3_finalize(stmt);

  sqlite3_close(db);
  return ret;
}



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