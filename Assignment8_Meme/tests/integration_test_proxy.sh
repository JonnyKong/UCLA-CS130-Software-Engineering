../build/bin/server ../tests/test_config_backend.conf &

pid_backend_server=$!
echo "The backend server starts with PID: $pid_backend_server"
sleep 1

../build/bin/server ../tests/test_config_frontend.conf &

pid_frontend_server=$!
echo "The frontend server starts with PID: $pid_frontend_server"
sleep 1

response1=$(echo -e "GET /local/static/data1.dat HTTP/1.1\r\n\r\n" | nc -q 1 localhost 8081)

ans1=$"HTTP/1.0 200 OK\r\nContent-Length: 20\r\nContent-Type: text/plain\r\n\r\nToo young, too naive"

diff <(echo "$response1") <(echo -e "$ans1")
result1=$?

kill -9 "$pid_frontend_server"
kill -9 "$pid_backend_server"

if [ $result1 != 0 ]
then
    echo "Connection failed"
    exit 1
fi

echo "Success"
exit 0
