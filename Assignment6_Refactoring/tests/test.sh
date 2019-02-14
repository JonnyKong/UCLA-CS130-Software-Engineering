#!/bin/bash
# if [ $# -ne 1 ]; then
#     echo "Usage: ./test.sh"
#     exit 1
# fi

RESPONSE_PATH=../tests/response
PATH_TO_BIN="./bin/server"

echo "
foo "bar";
server {
  port   8080;
  server_name foo.com;
  root /home/ubuntu/sites/foo/;
}" > example_config_test

$PATH_TO_BIN example_config_test &
pid_server=$!
echo $pid_server

sleep 1

# Test 1 should success:
response=$(printf '%s\r\n%s\r\n%s\r\n\r\n'  \
    "GET /echo HTTP/1.1"                        \
    "Host: www.example.com"                 \
    "Connection: close"                     \
    | nc 127.0.0.1 8080)
echo $response > test_response1
echo -n "Test 1 ... "
diff ${RESPONSE_PATH}/expected_response1 test_response1

if [[ $? -eq 0 ]]; then
    echo "success";
else
    echo "failed";
    kill -9 $pid_server
    exit 1;
fi


# Test 2 should fail with insufficient request headers:
response=$(printf '%s\r\n%s\r\n%s\r\n\r\n'  \
    "GET HTTP/1.1"                          \
    "Host: www.example.com"                 \
    "Connection: close"                     \
    | nc 127.0.0.1 8080)
echo $response > test_response2
echo -n "Test 2 ... "
if [[ $? -eq 0 ]]; then
    echo "success";
else
    echo "failed";
    kill -9 $pid_server
    exit 1;
fi


# Test 3 should not return without "\r\n\r\n" in the end (1s timeout)
unset response
response=$(printf '%s\r\n%s\r\n%s\r\n'      \
    "GET / HTTP/1.1"                        \
    "Host: www.example.com"                 \
    "Connection: close"                     \
    | nc 127.0.0.1 8080) &
pid=$!
sleep 1 && kill -9 $pid
echo -n "Test 3 ... "
if [[ $response = "" ]]; then
    echo "success";
else
    echo "failed";
    kill -9 $pid_server
    exit 1;
fi

kill -9 $pid_server
exit 0
