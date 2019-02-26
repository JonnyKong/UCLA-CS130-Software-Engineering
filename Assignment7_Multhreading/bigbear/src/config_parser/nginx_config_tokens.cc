#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include "nginx_config_tokens.h"


NginxConfigTokens::~NginxConfigTokens()
{
    for (Token* tkn : tokens)
    {
        delete tkn;
    }
}

// Factory function, only returns if valid config file
NginxConfigTokens* NginxConfigTokens::makeNginxConfigTokens(std::istream* input)
{
    NginxConfigTokens* toBeMade = new NginxConfigTokens;
    if(toBeMade->tokenizer(input))
    {
        return toBeMade;
    }
    delete toBeMade;
    return nullptr;
}

// Adds Token* to the list, resets the token state and the token type
void NginxConfigTokens::addToken(std::string& value, TokenType& type, TokenParserState& state)
{
    Token* tokenToAdd = new Token(value, type);
    tokens.push_back(tokenToAdd);
    value.clear();
    type = TOKEN_TYPE_UNFINISHED;
    state=TOKEN_STATE_INITIAL_WHITESPACE;
}

// This handles chars that signal a change in token state
TokenType NginxConfigTokens::handleStateChangeChars(TokenParserState& state,
                                                    TokenParserState newState, std::string& value,
                                                    char c, std::istream *input)
{
    switch (state)
    {
        case TOKEN_STATE_INITIAL_WHITESPACE:
            value = c;
            state = newState;
            return TOKEN_TYPE_UNFINISHED;
        case TOKEN_STATE_SINGLE_QUOTE:
            value += c;
            if(newState == TOKEN_STATE_SINGLE_QUOTE)
            {
                return TOKEN_TYPE_NORMAL;
            }
            else
            {
                return TOKEN_TYPE_ERROR;
            }
        case TOKEN_STATE_DOUBLE_QUOTE:
            value += c;
            if(newState == TOKEN_STATE_DOUBLE_QUOTE)
            {
                return TOKEN_TYPE_NORMAL;
            }
            else
            {
                return TOKEN_TYPE_ERROR;
            }
        case TOKEN_STATE_TOKEN_TYPE_COMMENT:
            value += c;
            return TOKEN_TYPE_UNFINISHED;
        case TOKEN_STATE_TOKEN_TYPE_NORMAL:
            input->unget();
            return TOKEN_TYPE_NORMAL;
    }
}

// This handles chars that should be found at the end of a statement
TokenType NginxConfigTokens::handleStatementEndChars(TokenParserState state, TokenType TYPE,
                                                     std::string& value, char c,
                                                     std::istream *input)
{
    switch (state)
    {
        case TOKEN_STATE_INITIAL_WHITESPACE:
            value = c;
            return TYPE;
        case TOKEN_STATE_SINGLE_QUOTE:
            value += c;
            break;
        case TOKEN_STATE_DOUBLE_QUOTE:
            value += c;
            break;
        case TOKEN_STATE_TOKEN_TYPE_NORMAL:
            input->unget();
            return TOKEN_TYPE_NORMAL;
        case TOKEN_STATE_TOKEN_TYPE_COMMENT:
            value += c;
            break;
        default:
            break;
    }
    return TOKEN_TYPE_UNFINISHED;
}

// This handles chars that are whitespaces
TokenType NginxConfigTokens::handleWhitespaceChars(TokenParserState state, std::string& value, char c, std::istream *input)
{
    switch (state)
    {
        case TOKEN_STATE_SINGLE_QUOTE:
            value += c;
            break;
        case TOKEN_STATE_DOUBLE_QUOTE:
            value += c;
            break;
        case TOKEN_STATE_TOKEN_TYPE_NORMAL:
            return TOKEN_TYPE_NORMAL;
        case TOKEN_STATE_TOKEN_TYPE_COMMENT:
            if(c == '\n' || c == '\r')
            {
                return TOKEN_TYPE_COMMENT;
            }
            break;
        default:
            break;
    }
    return TOKEN_TYPE_UNFINISHED;
}

// This handles every other type of char
void NginxConfigTokens::handleNormalChars(TokenParserState& state, std::string& value, char c)
{
    switch (state)
    {
        case TOKEN_STATE_INITIAL_WHITESPACE:
            value += c;
            state = TOKEN_STATE_TOKEN_TYPE_NORMAL;
            break;
        default:
            value += c;
            break;
    }
}

// This parses input file and separates out the tokens into an list
bool NginxConfigTokens::tokenizer(std::istream *input)
{
    // These maps are used both as a key/value pair and check to see if the char is present
    // by call the .count(char) function.
    std::unordered_map<char, int> stateChangeCharMap{ {'#', (int)TOKEN_STATE_TOKEN_TYPE_COMMENT},
                                                      {'"', (int)TOKEN_STATE_DOUBLE_QUOTE},
                                                      {'\'', (int)TOKEN_STATE_SINGLE_QUOTE}
    };
    std::unordered_map<char, int> statementEndCharMap{ {'{', (int)TOKEN_TYPE_START_BLOCK},
                                                       {'}', (int)TOKEN_TYPE_END_BLOCK},
                                                       {';', (int)TOKEN_TYPE_STATEMENT_END}
    };

    //Same function as a above, except no need for key/value here
    std::unordered_set<char> whiteSpaceTokenSet{' ', '\t', '\n', '\r'};

    TokenParserState state = TOKEN_STATE_INITIAL_WHITESPACE;
    std::string value;

    while (input->good()) {
        const char c = input->get();

        // Skips comments
        if(c == '#')
        {
            char comment;
            do
            {
                comment = input->get();
            } while(comment != '\n' && comment != '\r');
            continue;
        }

        // Used to ID the type of token
        TokenType type = TOKEN_TYPE_UNFINISHED;
        if (!input->good()) {
            break;
        }
        // if c == state change char
        if(stateChangeCharMap.count(c))
        {
            TokenParserState newState = (TokenParserState)stateChangeCharMap[c];
            type = handleStateChangeChars(state, newState, value, c, input);
        }
        // if c == statement end char
        else if(statementEndCharMap.count(c))
        {
            TokenType TYPE = (TokenType)statementEndCharMap[c];
            type = handleStatementEndChars(state, TYPE, value, c, input);
        }
        // if c == whitespace char
        else if(whiteSpaceTokenSet.count(c))
        {
            type = handleWhitespaceChars(state, value, c, input);
        }
        else
        {
            handleNormalChars(state, value, c);
        }
        if(type != TOKEN_TYPE_UNFINISHED)
        {
            addToken(value, type, state);
        }
    }
    // return true if there are not unclosed quotes
    return (state != TOKEN_STATE_SINGLE_QUOTE && state != TOKEN_STATE_DOUBLE_QUOTE);
}

std::deque<Token*> NginxConfigTokens::getTokens()
{
    return tokens;
}