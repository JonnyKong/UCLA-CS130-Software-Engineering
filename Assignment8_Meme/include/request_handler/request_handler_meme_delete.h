#ifndef REQUEST_HANDLER_MEME_DELETE_H
#define REQUEST_HANDLER_MEME_DELETE_H

#include <sqlite3.h>
#include <iostream>
#include <unordered_map>
#include "request_handler.h"
#include "meme_common.h"

class RequestHandlerMemeDelete : public RequestHandler {
    public:
        explicit RequestHandlerMemeDelete(const NginxConfig &config);
        RequestHandlerMemeDelete(){}
        std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;
    private:
        std::string database_name;
};
#endif  // REQUEST_HANDLER_MEME_VIEW_H