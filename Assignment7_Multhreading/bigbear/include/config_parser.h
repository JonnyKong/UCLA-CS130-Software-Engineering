#ifndef CONFIG_PARSER
#define CONFIG_PARSER

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include "nginx_config.h"
#include "nginx_enums.h"
#include "nginx_config_tokens.h"

using namespace NginxEnum;


// The driver that parses a config file and generates an NginxConfig.
// Returns a NginxConfig* with parameters for server.
class NginxConfigParser {
public:
    NginxConfigParser() = default;
    ~NginxConfigParser() = default;

    // Take a opened config file or file name (respectively) and store the
    // parsed config in the provided NginxConfig out-param.  Returns true
    // if the input config file is valid.
    // Also fills in parameters of NginxConfig* config to be returned by getConfig().
    bool Parse(std::istream* config_file, NginxConfig* conf);
    bool Parse(const char* file_name, NginxConfig* conf);


private:
    std::unordered_map<std::string,unsigned> handlersEncountered;

    std::string getHandlerName(std::string handler);

    bool setConfigBlock(NginxConfig* conf, std::deque<Token*>& TOKENS);
    void addFlatParamToConfig(NginxConfig *conf, std::deque<std::string> tokensInStatement);
    bool addNestedParamToConfig(NginxConfig* conf, std::deque<std::string> tokensInStatement, std::deque<Token*>& TOKENS);


    // Holds key words that open new context (new {}).
    // Key=context token, value=vector of parameters within that context.
    std::unordered_map<std::string, std::vector<std::string>> contextsMap;

    // Parameters that are to be stored in the returned config.
    // Key=parameter token, value=enum corresponding to parameter
    std::unordered_map<std::string, ConfigParameter> parametersMap;

    const char* TokenTypeAsString(TokenType type);
};

#endif //CONFIG_PARSER