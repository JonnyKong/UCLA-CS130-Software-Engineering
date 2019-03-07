/**
 * Request handler for accepting meme creation form.
 */

#ifndef REQUEST_HANDLER_MEME_CREATE_H
#define REQUEST_HANDLER_MEME_CREATE_H

#include <iostream>
#include <map>
#include <sqlite3.h>
#include <mutex>

#include "meme_common.h"
#include "request_handler.h"



class RequestHandlerMemeCreate : public RequestHandler {
    static std::mutex mtx;
protected:
    /* For unit testing mock class delegate constructor */
    explicit RequestHandlerMemeCreate() {}
public:
    explicit RequestHandlerMemeCreate(const NginxConfig &config);    /* To conform with sibling class */
    std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;
// Private:
    std::string insertToStorage(const MemeEntry &entry, int &id);
    std::string updateStorage(const MemeEntry &entry);
    static int sqlCount(void*data, int argc, char**argv, char**azColName);
    int getMaxId() noexcept;

    std::string database_name;
};

#endif  // REQUEST_HANDLER_MEME_CREATE_H
