#include <iostream>

#include "request_handler_dispatcher.h"
#include "request_handler/request_handler.h"
#include "request_handler/request_handler_static.h"
#include "request_handler/request_handler_echo.h"

/**
 * Constructor - Construct the RequestHandler set. After initialization, all
 *  members should remain immutable.
 */
RequestHandlerDispatcher::RequestHandlerDispatcher(const NginxConfig &config) {
    size_t num_registered = initRequestHandlers(config);
    // std::cout << "RequestHandlerDispatcher: Registered " << num_registered << " paths\n";
}


/**
 * getRequestHandler() - Given a request, return pointer to corresponding 
 *  request handler object with ***LONGEST PREFIX MATCH***.
 * 
 * When matching, trailing slashes should be removed, but preceding 
 *  slashes should be kept. However, "/" is a valid path.
 * 
 * For example, path "/static/" should be stipped to "/static".
 */
std::shared_ptr<RequestHandler> 
RequestHandlerDispatcher::getRequestHandler(const request &request_) const {

    // Remove trailing slashes
    PathUri uri = request_.uri;
    while (uri.length() > 1 && uri.back() == '/')
        uri.pop_back();
    std::shared_ptr<RequestHandler> matched_handler = nullptr;
    std::string matched_prefix;
    for (auto it = handlers_.begin(); it != handlers_.end(); ++it) {
        if (uri.substr(0, it->first.length()) == it->first) {   // Prefix match
            if (it->first.length() > matched_prefix.length()) { // Longest 
                matched_prefix = it->first;
                matched_handler = it->second;
            }
        }
    }
    return matched_handler;
}


/**
 * initRequestHandlers() - Called by constructor to init request handlers.
 *  Looks for "server" block, and parses child "location" blocks.
 * 
 * Return the number of RequestHandlers initialized.
 */
size_t RequestHandlerDispatcher::initRequestHandlers(const NginxConfig &config) {
    size_t num_registered = 0;
    for (int i = 0; i < config.statements_.size(); ++i) {
        if (config.statements_[i]->tokens_[0] == "server") {
            if (config.statements_[i]->tokens_.size() != 1)
                continue;   // Format error
            for (int j = 0; j < (config.statements_[i])->child_block_->statements_.size(); ++j) {
                if ((config.statements_[i])->child_block_->statements_[j]->tokens_.size() < 3)
                    continue;   // Format error
                if ((config.statements_[i])->child_block_->statements_[j]->tokens_[0] != "location")
                    continue;   // Format error
                if (registerPath((config.statements_[i])->child_block_->statements_[j]->tokens_[1], 
                                 (config.statements_[i])->child_block_->statements_[j]->tokens_[2], 
                                 *(config.statements_[i])->child_block_->statements_[j]->child_block_))
                    num_registered++;
            }
            break;
        }
    }
    return num_registered;
}


/**
 * registerPath() - Called by initRequestHandlers() to add one handler.
 *  Returns true if registration succeeds.
 * 
 * When registering paths, trailing slashes should be removed, but preceding 
 *  slashes should be kept. However, "/" is a valid path.
 * 
 * For example, path "/static/" should be stored as "/static".
 */
bool RequestHandlerDispatcher::registerPath(PathUri path_uri, HandlerType handler_type, 
                                            const NginxConfig &config) {
    // Remove trailing slashes
    while (path_uri.length() > 1 && path_uri.back() == '/')
        path_uri.pop_back();

    if (handlers_.find(path_uri) != handlers_.end()) {
        return false;   // Already registered
    }

    if (handler_type == StaticHandler)
        handlers_[path_uri] = std::make_shared<RequestHandlerStatic>(config, path_uri);
    else if (handler_type == EchoHandler)
        handlers_[path_uri] = std::make_shared<RequestHandlerEcho>(config);
    else
        return false;

    std::cout << "Path registered: " << path_uri << " as " << handler_type << std::endl;
}