#ifndef NGINX_ENUMS_H
#define NGINX_ENUMS_H


namespace NginxEnum {

    enum ServerAction
    {
        ACTION_ECHO = 0
    };

    enum ConfigParameter
    {
        LISTEN_PORT = 0,
        LOCATION = 1,
        ROOT = 3
    };

    enum TokenType {
        TOKEN_TYPE_UNFINISHED = -1,
        TOKEN_TYPE_START = 0,
        TOKEN_TYPE_NORMAL = 1,
        TOKEN_TYPE_START_BLOCK = 2,
        TOKEN_TYPE_END_BLOCK = 3,
        TOKEN_TYPE_COMMENT = 4,
        TOKEN_TYPE_STATEMENT_END = 5,
        TOKEN_TYPE_EOF = 6,
        TOKEN_TYPE_ERROR = 7
    };

    enum TokenParserState {
        TOKEN_STATE_INITIAL_WHITESPACE = 0,
        TOKEN_STATE_SINGLE_QUOTE = 1,
        TOKEN_STATE_DOUBLE_QUOTE = 2,
        TOKEN_STATE_TOKEN_TYPE_COMMENT = 3,
        TOKEN_STATE_TOKEN_TYPE_NORMAL = 4
    };
}

#endif //NGINX_ENUMS_H
