#!/bin/bash
# This shell script tests how server handles concurrency, by first sending
#   an incomplete request, and then send another request

PATH_TO_BIN="../build/bin/server"
PATH_TO_CONFIG="/tmp/example_config_test"

# Start server
echo "
port   8080;
worker 2;
root /user/src/project;
handler echo{
  location /echo;
}" > $PATH_TO_CONFIG
$PATH_TO_BIN $PATH_TO_CONFIG &
PID_SERVER=$!
sleep 1     # Wait for server to bootup completely

# Send the first request, with interval of 1 second between each line, so the 
#   full request will take 4 seconds.
printf '%s\r\n%s\r\n%s\r\n\r\n'                 \
    "GET /echo HTTP/1.1"                        \
    "Host: www.example1.com"                    \
    "Connection: close"                         \
    | nc -i 1 127.0.0.1 8080 &
PID_INCOMPLETE_REQUEST=$!

sleep 1
# Send another request, which should complete before the previous one. If this
#   test has response, it means the server handles concurrent requests.
RESPONSE=$(printf '%s\r\n%s\r\n%s\r\n\r\n'     \
    "GET /echo HTTP/1.1"                        \
    "Host: www.example2.com"                    \
    "Connection: close"                         \
    | nc 127.0.0.1 8080)
echo "Response 2:"
echo $RESPONSE


kill -9 $PID_SERVER
rm -f $PATH_TO_CONFIG
if [[ $RESPONSE != "" ]]; then
    echo "CONCURRENCY TEST SUCCESS"
    exit 0
else
    echo "CONCURRENCY TEST FAILED"
    exit 1
fi