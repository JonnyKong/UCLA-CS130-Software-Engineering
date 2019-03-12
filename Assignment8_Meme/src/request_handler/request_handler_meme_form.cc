#include <iostream>
#include <cassert>
#include <sqlite3.h>

#include "request_handler/request_handler_meme_form.h"

/**
 * Constructor: Maybe init the database and tables. The constructor doesn't need
 *  to take config as input.
 */
RequestHandlerMemeForm::RequestHandlerMemeForm(const NginxConfig &config)
  : database_name("../assets/meme.db") {}

/**
 * handleRequest() - Return an meme creation form.
 */
std::unique_ptr<reply> RequestHandlerMemeForm::handleRequest(const request &request_) noexcept {
  std::unique_ptr<reply> reply_ = std::make_unique<reply>();
  std::cout << "RequestHandlerMemeForm::handleRequest()" << std::endl;
  reply_->status = reply::ok;    // 200

  std::map<std::string, std::string> params = parseRESTParams(request_.uri);

  if (params.find("update") != params.end()) {

    // Invalid case 1: Empty param value
    if (params["update"] == "")
      return reply::stock_reply(reply::bad_request);
    // Invalid case 2: Value is not digit
    else if (!is_number(params["update"]))
      return reply::stock_reply(reply::bad_request);

    std::string id = params["update"];
    MemeEntry entry = fetchMemeEntry(id);

    // Invalid case 3: No such id in database
    if (entry.image == "")
      return reply::stock_reply(reply::bad_request);
    std::cout<<"go into the update case"<<std::endl;
    std::string image_str;
    if (entry.image == "simply.jpg")
      image_str = "one does not simply";
    else if (entry.image == "grumpy.jpg")
      image_str = "grumpy cat";
    else if (entry.image == "rubberduck")
      image_str = "rubberduck";

    // Fill form value for update
    form_ = "<form action=\"/meme/create\" method=\"GET\">"
      "<select name=\"image\" value=\"" + image_str + "\" selected>"
      "<option>Select a template...</option>"
      "<option value=\"simply.jpg\" " + (entry.image == "simply.jpg" ? "selected" : "") + ">one does not simply</option>"
      "<option value=\"grumpy.jpg\" " + (entry.image == "grumpy.jpg" ? "selected" : "") + ">grumpy cat</option>"
      "<option value=\"rubberduck\" " + (entry.image == "rubberduck" ? "selected" : "") + ">rubber duck</option>"
      "</select><br>"
      "<input type=\"text\" name=\"top\" placeholder=\"Top text...\" value=\"" + entry.top + "\" required><br>"
      "<input type=\"text\" name=\"bottom\" placeholder=\"Bottom text...\" value=\"" + entry.bottom + "\" required><br>"
      "<input type=\"hidden\" name=\"update\" value=\"" + id + "\">"
      "<input type=\"submit\" value=\"Update\">"
      "</form>";
  } else {
    // Return new empty form
    form_ = "<form action=\"/meme/create\" method=\"GET\">"
      "<select name=\"image\">"
      "<option>Select a template...</option>"
      "<option value=\"simply.jpg\">one does not simply</option>"
      "<option value=\"grumpy.jpg\">grumpy cat</option>"
       "<option value=\"rubberduck\">rubber duck</option>"
      "</select><br>"
      "<input type=\"text\" name=\"top\" placeholder=\"Top text...\" required><br>"
      "<input type=\"text\" name=\"bottom\" placeholder=\"Bottom text...\" required><br>"
      "<input type=\"submit\" value=\"Create\">"
      "</form>";
  }

  reply_->headers.resize(2);
  reply_->content = form_;
  reply_->headers[0].name = "Content-Length";
  reply_->headers[0].value = std::to_string((reply_->content).length());
  reply_->headers[1].name = "Content-Type";
  reply_->headers[1].value = "text/html";
std::cout<<"return reply"<<std::endl;
  return reply_;
}

/**
 * fetchMemeEntry() - Fetch meme entry from database using id.
 */
MemeEntry RequestHandlerMemeForm::fetchMemeEntry(const std::string &id) {
  sqlite3 *db;
  char *err_message = 0;
  int rc;
  std::vector<std::string> fetched_tuple;
  sqlite3_stmt *res;
  MemeEntry result("", "", "");

  rc = sqlite3_open(database_name.c_str(), &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(1);
  }

  const char *sql = "SELECT * "
                    "FROM   tbl1 "
                    "WHERE  id = ?1";
  rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
  if (rc == SQLITE_OK)
      sqlite3_bind_int(res, 1, std::stoi(id));
  else
      fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));

  int step = sqlite3_step(res);
  if (step == SQLITE_ROW) {
    // Discarding id in the first column
    result.image =  std::string(reinterpret_cast<const char*>(
      sqlite3_column_text(res, 1))
    );
    result.top =    std::string(reinterpret_cast<const char*>(
      sqlite3_column_text(res, 2))
    );
    result.bottom = std::string(reinterpret_cast<const char*>(
      sqlite3_column_text(res, 3))
    );
  }

  sqlite3_finalize(res);
  sqlite3_close(db);

  return result;
}

/**
 * is_number() - Verify that meme id is valid number.
 */
bool RequestHandlerMemeForm::is_number(const std::string& s) {
    return !s.empty() &&
      std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}
