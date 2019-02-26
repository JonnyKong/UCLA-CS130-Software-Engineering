#include <nginx_config.h>
#include <request_handler.h>


class HandlerManager {
    public:
        std::unique_ptr<RequestHandler> createByName(const std::string& name,
                                     const NginxConfig& config,
                                     const std::string& root_path);

};