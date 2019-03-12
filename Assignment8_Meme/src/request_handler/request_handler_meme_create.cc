#include "request_handler/request_handler_meme_create.h"
#include "session.h"
#include <cassert>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

/**
 * Constructor: Maybe init the database and tables. The constructor doesn't need
 *  to take config as input.
 */
RequestHandlerMemeCreate::RequestHandlerMemeCreate(const NginxConfig &config)
    : database_name("../assets/meme.db") {
  maybeInit(database_name);
}

/**
 * handleRequest() - Parse the request, and write to persistent store. Then,
 *  return a HTTP response as to whether the operation has succeeded.
 */
std::unique_ptr<reply>
RequestHandlerMemeCreate::handleRequest(const request &request_) noexcept {
  std::unique_ptr<reply> reply_ = std::make_unique<reply>();
  std::cout << "RequestHandlerMemeCreate::handleRequest()" << std::endl;

  std::map<std::string, std::string> params = parseRESTParams(request_.uri);

  // If no image parameter in params, return bad request
  auto it = params.find("image");
  if (it == params.end() || it->second.empty()) {
    reply_ = http::server::reply::stock_reply(reply::bad_request);
    return reply_;
  }

  // If no top/bottom in params, set to empty
  if (params.find("top") == params.end())
    params["top"] = "";
  if (params.find("bottom") == params.end())
    params["bottom"] = "";

  MemeEntry entry(escape(params["image"]), escape(params["top"]),
                  escape(params["bottom"]));
  std::replace(entry.top.begin(), entry.top.end(), '+', ' ');
  std::replace(entry.bottom.begin(), entry.bottom.end(), '+', ' ');
  int id;
  std::string ret, display_html_content;
  std::cout<<"try to find update"<<std::endl;
  if (params.find("update") == params.end()) {
    std::cout<<"did not find out meme"<<std::endl;
    ret = insertToStorage(entry, id);
    display_html_content = "<html>\n"
                           "<title>Created meme!</title>\n"
                           "<body> Meme Created! <a href=\"/meme/view?id=" +
                           std::to_string(id) + "\">" +
                           "Meme ID: " + std::to_string(id) +
                           "</a>"
                           "</body>\n"
                           "</html>\n";
  } else {
    std::cout<<"find out meme"<<std::endl;
    entry.id = std::stoi(params["update"]);
    id = entry.id;
    ret = updateStorage(entry);
    display_html_content = "<html>\n"
                           "<title>Updated meme!</title>\n"
                           "<body> Meme Updated! <a href=\"/meme/view?id=" +
                           std::to_string(id) + "\">" +
                           "Meme ID: " + std::to_string(id) +
                           "</a>"
                           "</body>\n"
                           "</html>\n";
  }

  if (ret != "SUCCESS")
    reply_ = http::server::reply::stock_reply(reply::bad_request);
  else {
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
 * insertToStorage() - Insert this entry into persistent storage. Because the
 *  id of this entry is unknown until the insertion is completed, this field is
 *  left empty.
 */
std::string RequestHandlerMemeCreate::insertToStorage(const MemeEntry &entry,
                                                      int &id) {
  std::lock_guard<std::mutex> lock(mtx);
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
  const char *sql_query = "INSERT INTO tbl1 "
                          "(image, top, bottom) "
                          "VALUES(?1, ?2, ?3)";
  sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, entry.image.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, entry.top.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, entry.bottom.c_str(), -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rc != SQLITE_DONE) {
    // ret = fmt::sprintf("ERROR inserting data: %s\n", sqlite3_errmsg(db));
    ret = std::string("ERROR inserting data: ") +
          std::string(sqlite3_errmsg(db)) + "\n";

    id = -1;
  } else {
    ret = "SUCCESS";
    id = getMaxId();
  }
  sqlite3_close(db);
  std::cout << "Created one database entry" << std::endl;
  return ret;
}

/**
 * insertToStorage() - Update entry in the database. Identify this database
 *  entry by id, and update the remaining fields to the given values.
 */
std::string RequestHandlerMemeCreate::updateStorage(const MemeEntry &entry) {
  std::cout<<"update called"<<std::endl;
  std::lock_guard<std::mutex> lock(mtx);
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

  // Update record
  const char *sql_query = "UPDATE tbl1 "
                          "SET image = ?1, top = ?2, bottom = ?3"
                          "WHERE id = ?4";
  sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, entry.image.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, entry.top.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, entry.bottom.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 4, entry.id);
  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rc != SQLITE_DONE) {
    ret = std::string("ERROR inserting data: ") +
          std::string(sqlite3_errmsg(db)) + "\n";
  } else {
    ret = "SUCCESS";
  }
  sqlite3_close(db);
  std::cout << "Updated one database entry" << std::endl;
  return ret;
}

/* getMaxId() - Get the id of entry just inserted */
int RequestHandlerMemeCreate::getMaxId() noexcept {
  sqlite3 *db;
  char *err_message = 0;
  int rc;
  char *sql;
  int max_id;
  rc = sqlite3_open(database_name.c_str(), &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(1);
  }
  // find the record
  std::string sql_resp;
  std::string query_str = "SELECT MAX(id) FROM tbl1";
  const char *sql_query = query_str.c_str();
  rc = sqlite3_exec(db, sql_query, sqlCount, (void *)(&max_id), &err_message);
  sqlite3_close(db);
  return max_id;
}

int RequestHandlerMemeCreate::sqlCount(void *data, int argc, char **argv,
                                       char **azColName) {
  int *tbl_cnt = (int *)data;
  if (argc == 0)
    return -1;
  for (int i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    *tbl_cnt = std::atoi(argv[i]);
    return 1;
  }
  return 1;
};
