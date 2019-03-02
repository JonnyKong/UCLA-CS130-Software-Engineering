#ifndef REQUEST_HANDLER_MEME_VIEW_H
#define REQUEST_HANDLER_MEME_VIEW_H

#include "request_handler.h"

class RequestHandlerMemeView : public RequestHandler {
    public:
        explicit RequestHandlerMemeView(const NginxConfig &config);
        std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;
}


#endif