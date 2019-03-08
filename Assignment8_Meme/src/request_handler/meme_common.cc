#include <iostream>
#include <sqlite3.h> 
#include <map>
#include <boost/algorithm/string/replace.hpp>

#include "request_handler/meme_common.h"

/**
 * createTableIfNotExists() - When the server is deployed to a new server, need
 *  to create the database and table. This function is called by the constructor.
 */
void maybeInit(std::string database_name) {
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
                          "id     INTEGER PRIMARY KEY AUTOINCREMENT, "
                          "image  varchar(100), "
                          "top    varchar(100), "
                          "bottom varchar(100), "
                          "UNIQUE (image, top, bottom)"
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
 * parseRESTParams() - Given an RESTful URI, return a map of its params.
 */
std::map<std::string, std::string> parseRESTParams(const std::string &uri) {
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
    params[param] = urlDecode(value);
    cursor = (cursor_next_param == std::string::npos) ? 
             std::string::npos : cursor_next_param + 1;
  }
  return params;
}


/**
 * urlDecode() - Given an url in percentage encoding, return the decoded string.
 *  This function is from StackOverflow.
 */
std::string urlDecode(const std::string &url_encoded) {
  std::string ret;
  char ch;
  int i, ii;
  for (i = 0; i < url_encoded.length(); i++) {
    if (url_encoded[i] == '%') {
      sscanf(url_encoded.substr(i + 1,2).c_str(), "%x", &ii);
      ch = static_cast<char>(ii);
      ret += ch;
      i = i + 2;
    } else if (url_encoded[i] == '+') {
      ret += ' ';
    } else {
      ret += url_encoded[i];
    }
  }
  return (ret);
}

/**
 * escape() - Escape user input that may cause html injection. All user inputs
 *  should be processed by this function.
 */
std::string escape(const std::string &data) {
  using boost::algorithm::replace_all;
  std::string ret(data);
  replace_all(ret, "&",  "&amp;");
  replace_all(ret, "\"", "&quot;");
  replace_all(ret, "\'", "&apos;");
  replace_all(ret, "<",  "&lt;");
  replace_all(ret, ">",  "&gt;");
  return ret;
}