#ifndef REQUEST_HANDLER_MEME_VIEW_H
#define REQUEST_HANDLER_MEME_VIEW_H

#include <sqlite3.h>
#include <iostream>
#include <unordered_map>
#include "request_handler.h"
#include "meme_common.h"

class RequestHandlerMemeView : public RequestHandler {
    public:
        explicit RequestHandlerMemeView(const NginxConfig &config);
        RequestHandlerMemeView(){}
        std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;
        static int sqlCallback(void*data, int argc, char**argv, char**azColName);
        std::string fetchImage(const std::string &id, 
                               const std::string &top_txt, 
                               const std::string &bottom_txt,
                               const std::string &img_uri);
        std::string database_name;
        std::unordered_map<std::string, std::string> name2uri;
};
#endif  // REQUEST_HANDLER_MEME_VIEW_H