#include <stack>
#include "config_parser.h"


void NginxConfigParser::addFlatParamToConfig(NginxConfig *conf, std::deque<std::string> tokensInStatement)
{
    if(tokensInStatement.empty())
    {
        return;
    }
    std::string paramName = tokensInStatement.front();
    tokensInStatement.pop_front();
    std::string parameterValue;

    // For if a parameter value is multi-word
    while (!tokensInStatement.empty())
    {
        parameterValue += tokensInStatement.front();
        parameterValue += " ";
        tokensInStatement.pop_front();
    }
    if(!parameterValue.empty())
    {
        parameterValue.pop_back();
    }

    conf->addFlatParam(paramName, parameterValue);
}

std::string NginxConfigParser::getHandlerName(std::string handler)
{
    unsigned numEncountered = 1;

    if(handlersEncountered.count(handler))
    {
        numEncountered += handlersEncountered[handler]++;
    }
    else
    {
        handlersEncountered.insert( {handler, numEncountered} );
    }
    // return handler name + how many times its been seen
    return (handler + std::to_string(numEncountered));
}

bool NginxConfigParser::addNestedParamToConfig(NginxConfig *conf, std::deque<std::string> tokensInStatement,
                                               std::deque<Token*>& TOKENS)
{
    bool isBlockSet = false;
    if(tokensInStatement.size() == 2)
    {
        // context is a request handler
        if(tokensInStatement.front() == "handler")
        {
            std::string handlerName = tokensInStatement.back();
            std::string enumeratedHandlerName = getHandlerName(handlerName);
            NginxConfig* handlerConfig = new NginxConfig(enumeratedHandlerName);
            conf->addNestedParam(enumeratedHandlerName, handlerConfig);
            // recursive call to set inner config
            isBlockSet = setConfigBlock(handlerConfig, TOKENS);
        }
    }
    // any context other than request handler
    else if(tokensInStatement.size() == 1)
    {
        NginxConfig* nestedConfig = new NginxConfig(tokensInStatement.front());
        conf->addNestedParam(tokensInStatement.front(), nestedConfig);
        // recursive call to set inner config
        isBlockSet = setConfigBlock(nestedConfig, TOKENS);
    }
    return isBlockSet;
}

bool NginxConfigParser::setConfigBlock(NginxConfig* conf, std::deque<Token*>& TOKENS)
{
    // currTokenValue == '{'
    std::string currTokenValue = TOKENS.front()->value;
    TokenType currTokenType = TOKENS.front()->type;
    TOKENS.pop_front();

    if(currTokenType != TOKEN_TYPE_START_BLOCK) { return false; }

    // currTokenValue == first word of statement
    currTokenValue = TOKENS.front()->value;
    currTokenType = TOKENS.front()->type;

    std::deque<std::string> tokensInStatement;

    while(!TOKENS.empty() && currTokenType != TOKEN_TYPE_END_BLOCK)
    {
        while(!TOKENS.empty() && currTokenType == TOKEN_TYPE_NORMAL)
        {
            // collect all normal tokens until ';' or '{' is reached
            tokensInStatement.push_back(currTokenValue);
            TOKENS.pop_front();
            currTokenValue = TOKENS.front()->value;
            currTokenType = TOKENS.front()->type;
        }

        switch(currTokenType)
        {
            case TOKEN_TYPE_STATEMENT_END:
                addFlatParamToConfig(conf, tokensInStatement);
                break;
            case TOKEN_TYPE_START_BLOCK:
                if(!addNestedParamToConfig(conf, tokensInStatement, TOKENS)) { return false; }
                break;
            default:
                return false;
        }
        //clear previous statement
        tokensInStatement.clear();

        TOKENS.pop_front();
        if(TOKENS.empty()) { return false; }
        currTokenValue = TOKENS.front()->value;
        currTokenType = TOKENS.front()->type;
    }
    // last token in context must be '}'
    return (currTokenType == TOKEN_TYPE_END_BLOCK);
}

bool NginxConfigParser::Parse(std::istream* config_file, NginxConfig* conf)
{
    NginxConfigTokens* tokenList = NginxConfigTokens::makeNginxConfigTokens(config_file);
    if(tokenList == nullptr) { return false; }

    std::deque<Token*> TOKENS = tokenList->getTokens();

    // only allow for configs that start with 'server' context
    if(TOKENS.front()->value != "server") { return false; }
    TOKENS.pop_front();

    return setConfigBlock(conf, TOKENS);

}

bool NginxConfigParser::Parse(const char* file_name, NginxConfig* conf) {
    std::ifstream config_file;
    config_file.open(file_name);
    if (!config_file.good()) {
        printf ("Failed to open config file: %s\n", file_name);
        return false;
    }

    const bool return_value =
            Parse(dynamic_cast<std::istream*>(&config_file), conf);
    config_file.close();
    return return_value;
}

const char* NginxConfigParser::TokenTypeAsString(TokenType type) {
    switch (type) {
        case TOKEN_TYPE_START:         return "TOKEN_TYPE_START";
        case TOKEN_TYPE_NORMAL:        return "TOKEN_TYPE_NORMAL";
        case TOKEN_TYPE_START_BLOCK:   return "TOKEN_TYPE_START_BLOCK";
        case TOKEN_TYPE_END_BLOCK:     return "TOKEN_TYPE_END_BLOCK";
        case TOKEN_TYPE_COMMENT:       return "TOKEN_TYPE_COMMENT";
        case TOKEN_TYPE_STATEMENT_END: return "TOKEN_TYPE_STATEMENT_END";
        case TOKEN_TYPE_EOF:           return "TOKEN_TYPE_EOF";
        case TOKEN_TYPE_ERROR:         return "TOKEN_TYPE_ERROR";
        default:                       return "Unknown token type";
    }
}