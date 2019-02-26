#ifndef HTTP_REQUEST_ENUMS_H
#define HTTP_REQUEST_ENUMS_H


namespace httpRequestEnum {

    enum httpMethod 
    {
        GET = 0,
        HEAD = 1,
        POST = 2,
        PUT = 3,
        DELETE = 4,
        CONNECT = 5,
        OPTIONS = 6,
        TRACE = 7,
        PATCH = 8,
    };
    // TODO: add support for other header fields as needed
    enum httpHeaderFields 
    {
        ACCEPT = 0,
        ACCEPT_CHARSET = 1,
        ACCEPT_ENCODING = 2,
        ACCEPT_LANGUAGE = 3,
        CONNECTION = 4,
        CONTENT_LENGTH = 5,
        HOST = 6,
        USER_AGENT = 7,
        UPGRADE_INSECURE_REQUESTS = 8,
        CACHE_CONTROL = 9,
    };
}

#endif //HTTP_REQUEST_ENUMS_H
