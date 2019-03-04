#include <iostream>
#include <sqlite3.h> 
#include <map>

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