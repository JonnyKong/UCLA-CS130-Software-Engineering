printf '%s\r\n%s\r\n%s\r\n\r\n'             \
    "GET /meme/view/2 HTTP/1.1"                        \
    "Host: www.example.com"                 \
    "Connection: close"                     \
    | nc 127.0.0.1 8080
