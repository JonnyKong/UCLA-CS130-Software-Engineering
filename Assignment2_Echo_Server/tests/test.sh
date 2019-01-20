#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: ./test.sh <port>"
    exit 1
fi

printf 'GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n' \
    | nc localhost $1