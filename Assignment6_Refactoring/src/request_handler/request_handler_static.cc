#include <iostream>

#include "request_handler/request_handler_static.h"

/**
 * constructor - If no root in config string, use "/" as default.
 */
RequestHandlerStatic::RequestHandlerStatic(const NginxConfig &config, const PathUri &prefix_)
    : prefix(prefix_) {

    root = "/";
    for (int i = 0; i < config.statements_.size(); ++i) {
        if (config.statements_[i]->tokens_[0] == "root" &&
                config.statements_[i]->tokens_.size() == 2) {
            root = config.statements_[i]->tokens_[1];
        }
    }
    std::cout << root << std::endl;
}

/**
 * handleRequest() - Fill reply with static files.
 */
std::unique_ptr<reply> RequestHandlerStatic::handleRequest(const request &request) noexcept {
    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    // Substitute matched prefix with root
    std::string uri = request.uri;
    uri.replace(0, prefix.length(), root);
    uri.replace(0, 1, "../");    // Change to relative path     // TODO: A more elegant solution?
    std::cout << "RequestHandlerStatic::handleRequest() Serving file: " << uri << std::endl;

    // Serve file
    boost::filesystem::path boost_path(uri);
    if (!boost::filesystem::exists(uri) || !boost::filesystem::is_regular_file(uri)) {
        reply_ = http::server::reply::stock_reply(reply::not_found);
        return reply_;
    }

    std::ifstream f(uri.c_str(), std::ios::in | std::ios::binary);
    if (!f) {
        reply_ = http::server::reply::stock_reply(reply::not_found);
        return reply_;
    }

    // Read file
    std::string body;
    char c;
    while (f.get(c)) body += c;
    f.close();

    // Use extension to get MIME types
    std::string extension;
    size_t cursor = uri.find_last_of(".");
    if (cursor != std::string::npos)
        extension = uri.substr(cursor + 1);

    reply_->status = reply::ok;
    reply_->headers.resize(2);
    reply_->content = body;
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string(body.length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = http::server::mime_types::extension_to_type(extension);
    return reply_;
}
