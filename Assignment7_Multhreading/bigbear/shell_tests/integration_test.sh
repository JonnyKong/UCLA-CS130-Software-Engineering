#!/bin/bash

function test_response
{
    response=$(curl -s localhost:5566$2)
    if [ "$response" != "$3" ]; then
        echo "$2 Response is NOT correct"
        echo -n "$response" > response.txt
        echo -n "$expected_response" > expected.txt
        kill $1
        exit 1
    fi
}

../build/bin/server ./test_config &
SERVER_PID=$!
sleep .5

# test echo response
URI="/echo"
expected_response=$'GET /echo HTTP/1.1\r\nHost: localhost:5566\r\nUser-Agent: curl/7.58.0\r\nAccept: */*\r\n\r'
test_response $SERVER_PID $URI "$expected_response"
echo "Echo response is correct"

# test bad-request response
URI="/bad"
expected_response=$'404 not found\r'
test_response $SERVER_PID $URI "$expected_response"
echo "Bad-request response is correct"

# test static .txt file response
URI="/static/bearlist.txt"
expected_response=$'Here is a list of bears:\n\nGrizzly Bear\nPanda Bear\nGummy Bear\nKoala Bear\nYogi Bear\nSmokey The Bear\nPolar Bear'
test_response $SERVER_PID $URI "$expected_response"
echo "Static .txt response is correct"

# test 404 error response
URI="/static/notFound.txt"
expected_response=$'<!DOCTYPE html>\n<html lang="en">\n<head>\n    <meta charset="UTF-8">\n    <title>404</title>\n</head>\n<body>\n404 - File Not Found\n</body>\n</html>'
test_response $SERVER_PID $URI "$expected_response"
echo "404 error response is correct"

# test proxy
URI="/moved_echo"
expected_response="Hello Node.js Server!"
test_response $SERVER_PID $URI "$expected_response"
echo "reverse proxy is correct"


kill $SERVER_PID
exit 0
