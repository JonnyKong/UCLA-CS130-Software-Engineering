/**
 * Request handler for showing all memes.
 */

#ifndef REQUEST_HANDLER_MEME_LIST_H
#define REQUEST_HANDLER_MEME_LIST_H

#include <iostream>
#include <sqlite3.h> 

#include "request_handler.h"
#include "meme_common.h"
#include <boost/format.hpp>




class RequestHandlerMemeList : public RequestHandler {
protected:
    /* For unit testing mock class delegate constructor */
    explicit RequestHandlerMemeList() {}
public:
    explicit RequestHandlerMemeList(const NginxConfig &config);    /* To conform with sibling class */
    std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;

// Private:
    std::vector<MemeEntry> selectAllMeme();
    static int appendMeme(void *data, int argc, char **argv, char **azColName);

    std::string database_name;  // TODO: Use absolute path?
};

#endif  // REQUEST_HANDLER_MEME_LIST_H
