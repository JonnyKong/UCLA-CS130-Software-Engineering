/**
 * Request handler for serving static files.
 * 
 * RequestHandlerStatic have to be explicitly initialized with a file root path.
 */

#ifndef REQUEST_HANDLER_STATIC_H
#define REQUEST_HANDLER_STATIC_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "request_handler.h"
#include "../request_handler_dispatcher.h"  // Include typedefs
#include "../http/mime_types.h"


class RequestHandlerStatic : public RequestHandler {
public:
    explicit RequestHandlerStatic(const NginxConfig &config, const PathUri &prefix_);
    std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;


private:
    PathUri root;   /* Root path to serve files from */
    PathUri prefix; /* Need to know prefix to substitute it with root */
};

#endif  // REQUEST_HANDLER_STATIC_H