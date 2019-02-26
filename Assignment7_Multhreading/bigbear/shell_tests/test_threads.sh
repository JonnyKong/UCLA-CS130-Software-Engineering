#!/bin/bash

# fifo is named pipe
mkfifo fifo

# Wait for server to start up
../build/bin/server ./test_config > /dev/null &
SERVER_PID=$!
sleep .5

# Keep fifo open for multiple reads/writes
exec 3<> fifo

# Start up netcat connection to server
# reads from fifo, writes to 'threads'
nc localhost 5566 <fifo >> threads &
NC_PID=$!
sleep .5

# Send first half of a request to netcat
printf "GET /static/second.txt HTTP/1.1\r\nUser-Agent:" >&3
sleep .5

# Send full request and write response to 'threads'
curl -s localhost:5566/static/first.txt >> threads

# Send second half of request to netcat
printf "nc/0.0.1\r\nHost: 127.0.0.1\r\nAccept: */*\r\n\r\n" >&3

# close and remove fifo
exec 3>&-
rm fifo

first="$(grep -n "FIRST" threads)"
second="$(grep -n "SECOND" threads)"
echo $first
echo $second
rm threads

# get line number of each response
first_linenumber="$(echo $first | head -c 1)"
second_linenumber="$(echo $second | head -c 1)"

kill $NC_PID
kill $SERVER_PID

# curl response should come before netcat response
if [ "$first_linenumber" -gt "$second_linenumber" ]; then
        echo "Responses out of order."
        exit 1
fi

echo "Responses in order."
exit 0