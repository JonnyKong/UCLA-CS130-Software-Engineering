echo "================= Building source codes =================="
if [ -d "build" ]; then
  echo "Deleting the old build folder......"
  rm -r build
fi
mkdir build
cd build
cmake ..
make

# echo "======================= Run tests ========================"
# cd ../tests && ../build/bin/request_handler_proxy_test

# # echo "================== Starting the server ==================="
# # ../build/bin/server ../conf/test.conf

# echo "================ Get test coverage report ================"
# if [ -d "build_coverage" ]; then
#   echo "Deleting the old build_coverage folder......"
#   rm -r build_coverage
# fi
# mkdir build_coverage
# cd build_coverage
# cmake -DCMAKE_BUILD_TYPE=Coverage ..
# make coverage