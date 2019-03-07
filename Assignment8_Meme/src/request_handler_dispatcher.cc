#include <iostream>

#include "request_handler_dispatcher.h"
#include "request_handler/request_handler.h"
#include "request_handler/request_handler_static.h"
#include "request_handler/request_handler_echo.h"
#include "request_handler/request_handler_error.h"
#include "request_handler/request_handler_status.h"
#include "request_handler/request_handler_proxy.h"
#include "request_handler/request_handler_redirect.h"
#include "request_handler/request_handler_meme_form.h"
#include "request_handler/request_handler_meme_view.h"
#include "request_handler/request_handler_meme_create.h"
#include "request_handler/request_handler_meme_delete.h"
#include "request_handler/request_handler_meme_list.h"
#include "request_handler/request_handler_health.h"

/**
 * Constructor - Construct the RequestHandler set. After initialization, all
 *  members should remain immutable.
 */
RequestHandlerDispatcher::RequestHandlerDispatcher(const NginxConfig &config) {
    size_t num_registered = initRequestHandlers(config);
    // std::cout << "RequestHandlerDispatcher: Registered " << num_registered << " paths\n";
}


/**
 * getRequestHandler() - Given a request, return a pointer to corresponding
 *  request handler object with ***LONGEST PREFIX MATCH***.
 *
 * When matching, trailing slashes should be removed, but preceding
 *  slashes should be kept. However, "/" is a valid path.
 *
 * For example, path "/static/" should be stipped to "/static".
 *
 * This is FACTORY pattern.
 */
std::unique_ptr<RequestHandler>
RequestHandlerDispatcher::getRequestHandler(const request &request_) const {

    // Remove trailing slashes
    PathUri uri = request_.uri;
    while (uri.length() > 1 && uri.back() == '/')
        uri.pop_back();
    std::shared_ptr<RequestHandler> matched_handler = nullptr;
    std::string matched_prefix;
    for (auto it = handler_configs_.begin(); it != handler_configs_.end(); ++it) {
        if (uri.substr(0, it->first.length()) == it->first) {   // Prefix match
            if (it->first.length() > matched_prefix.length()) { // Longest
                matched_prefix = it->first;
            }
        }
    }

    if (handler_configs_.find(matched_prefix) == handler_configs_.end()) {
        // Failed to match to any handler
        NginxConfigStatement empty_statement;
        return std::make_unique<RequestHandlerError>(
            NginxConfig()
        );
    }
    else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == StaticHandler) {
        return std::make_unique<RequestHandlerStatic>(
            *(handler_configs_.find(matched_prefix)->second->child_block_),
            matched_prefix
        );
    }
    else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == EchoHandler) {
        return std::make_unique<RequestHandlerEcho>(
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
    }

    else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == StatusHandler) {
        return std::make_unique<RequestHandlerStatus>(
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
    }

    else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == ProxyHandler) {
        return std::make_unique<RequestHandlerProxy>(
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
    }

    else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == RedirectHandler) {
        return std::make_unique<RequestHandlerRedirect>(
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
    }

    else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == MemeFormHandler) {
        return std::make_unique<RequestHandlerMemeForm>(
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
    }

    else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == MemeCreateHandler) {
        return std::make_unique<RequestHandlerMemeCreate>(
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
    }

    else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == MemeDeleteHandler) {
        return std::make_unique<RequestHandlerMemeDelete> (
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
    }

    else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == MemeViewHandler) {
        return std::make_unique<RequestHandlerMemeView> (
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
    }

    else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == MemeListHandler) {
        return std::make_unique<RequestHandlerMemeList> (
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
    }

    else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == HealthHandler) {
        return std::make_unique<RequestHandlerHealth> (
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
    }

    // Wouldn't actually get here. Keep GCC happy
    return std::make_unique<RequestHandlerError>(
        NginxConfig()
    );
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

        if (config.statements_[i]->tokens_.size() != 2)
            continue;   // Format error

        if ((config.statements_[i])->tokens_[0] != "handler")
            continue;   // Format error

        if (registerPath((config.statements_[i])->tokens_[1],
                         config.statements_[i]))
            num_registered++;

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
bool RequestHandlerDispatcher::registerPath(HandlerType handler_type,
                                            std::shared_ptr<const NginxConfigStatement> statement) {
    if (statement->child_block_ == nullptr)
        return false;   // No child block
    else if (statement->tokens_.size() != 2 ||
             statement->tokens_[0] != "handler")
        return false;   // Invalid format

    // Retrieve path from child block. If there are multiple paths in this block,
    //  only use the first one.
    PathUri path;
    for (int i = 0; i < statement->child_block_->statements_.size(); ++i) {
        if (statement->child_block_->statements_[i]->tokens_.size() == 2 &&
            statement->child_block_->statements_[i]->tokens_[0] == "location") {
            path = statement->child_block_->statements_[i]->tokens_[1];
            break;
        }
    }
    // Remove trailing slashes
    while (path.length() > 1 && path.back() == '/')
        path.pop_back();

    if (path == "")
        return false;   // No path in block, invalid
    else if (handler_configs_.find(path) != handler_configs_.end())
        return false;   // Already registered
    handler_configs_[path] = statement;

    std::cout << "Registered handler: " << path << std::endl;
    return true;
}

std::map<PathUri, std::shared_ptr<const NginxConfigStatement>> RequestHandlerDispatcher::handler_configs_;
