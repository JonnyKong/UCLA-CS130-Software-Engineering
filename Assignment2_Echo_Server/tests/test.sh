#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: ./test.sh <port>"
    exit 1
fi

# Test 1 should success:
response=$(printf '%s\r\n%s\r\n%s\r\n\r\n'  \
    "GET / HTTP/1.1"                        \
    "Host: www.example.com"                 \
    "Connection: close"                     \
    | nc 35.247.29.163 $1)
answer=$(printf '%s\r\n%s\r\n%s\r\n\r\n%s\r\n%s\r\n%s\r\n\r\n'  \
    "HTTP/1.0 200 OK"                                           \
    "Content-Length: 60"                                        \
    "Content-Type: text/plain"                                  \
    "GET / HTTP/1.1"                                            \
    "Host: www.example.com"                                     \
    "Connection: close")
echo -n "Test 1 ... "
if [[ $response = $answer ]]; then echo "success"; else echo "failed"; fi


# Test 2 should fail with insufficient request headers:
response=$(printf '%s\r\n%s\r\n%s\r\n\r\n'  \
    "GET HTTP/1.1"                          \
    "Host: www.example.com"                 \
    "Connection: close"                     \
    | nc 35.247.29.163 $1)
answer=$(printf '%s\r\n%s\r\n%s\r\n\r\n%s'  \
    "HTTP/1.0 400 Bad Request"              \
    "Content-Length: 89"                    \
    "Content-Type: text/html"               \
    "<html><head><title>Bad Request</title></head><body><h1>400 Bad Request</h1></body></html>")
echo -n "Test 2 ... "
if [[ $response = $answer ]]; then echo "success"; else echo "failed"; fi


# Test 3 should not return without "\r\n\r\n" in the end (1s timeout)
unset response
response=$(printf '%s\r\n%s\r\n%s\r\n'      \
    "GET / HTTP/1.1"                        \
    "Host: www.example.com"                 \
    "Connection: close"                     \
    | nc 35.247.29.163 $1) &
pid=$!
sleep 1 && kill -9 $pid
echo -n "Test 3 ... "
if [[ $response = "" ]]; then echo "success"; else echo "failed"; fi
