printf '%s\r\n%s\r\n%s\r\n\r\n'             \
    "GET /meme/create?image=Tom&top=myTop&bottom=myBottom HTTP/1.1"                        \
    "Host: www.example.com"                 \
    "Connection: close"                     \
    | nc 127.0.0.1 8080