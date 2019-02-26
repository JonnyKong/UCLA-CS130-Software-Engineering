cmake_minimum_required(VERSION 3.10.0)
project(bigbear)

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()

# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
    message(STATUS "Setting debug build type by default in devel shell")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.50 REQUIRED COMPONENTS system log regex)
message(STATUS "Boost version: ${Boost_VERSION}")
message("Boost VERSION: ${Boost_VERSION}")
message("Boost INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message("Boost Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
message("Boost LIBRARIES: ${Boost_LIBRARIES}")
include_directories(include)

# TODO(!): Update name and srcs
add_library(config_parser_lib src/config_parser/config_parser.cc src/config_parser/nginx_config_tokens.cc src/config_parser/nginx_config.cc)
add_library(server_lib src/server.cc src/session.cc src/http_request.cc src/http_response.cc src/static_file_request_handler.cc src/action_request_handler.cc)
add_library(request_handler_lib src/proxy_request_handler.cc src/static_file_request_handler.cc src/action_request_handler.cc src/error_request_handler.cc src/status_request_handler.cc src/redirect_request_handler.cc src/handler_manager.cc src/request_handler_dispatcher.cc)
add_library(http_response src/http_response.cc)
add_library(http_request src/http_request.cc)
target_link_libraries(request_handler_lib config_parser_lib)

# TODO(!): Update executable name, srcs, and deps
add_executable(server src/server_main.cc src/server.cc src/session.cc src/http_request.cc src/http_response.cc)
target_link_libraries(server server_lib config_parser_lib request_handler_lib Boost::system)
target_link_libraries(server Boost::log_setup Boost::log Boost::regex)
target_link_libraries(server Threads::Threads)

# build and link session_test
add_executable(session_test tests/server_tests/session_test.cc src/static_file_request_handler.cc)
target_link_libraries(session_test server_lib request_handler_lib gmock_main Boost::log_setup Boost::log Boost::regex Boost::system)

# build and link server_test
add_executable(server_test tests/server_tests/server_test.cc)
target_link_libraries(server_test request_handler_lib server_lib Boost::system gmock_main Boost::log_setup Boost::log Boost::regex)

# TODO(!): Update tests executable name, srcs, and deps
add_executable(config_parser_test tests/config_parser_tests/config_parser_test.cc tests/config_parser_tests/nginx_config_tokens_test.cc)
target_link_libraries(config_parser_test config_parser_lib gtest_main)

add_executable(static_file_request_handler_test tests/request_handler_tests/static_file_request_handler_test.cc)
target_link_libraries(static_file_request_handler_test request_handler_lib http_response http_request gtest_main Boost::system Boost::log Boost::regex)

add_executable(action_request_handler_test tests/request_handler_tests/action_request_handler_test.cc)
target_link_libraries(action_request_handler_test request_handler_lib http_response http_request gtest_main Boost::system Boost::log Boost::regex)

add_executable(proxy_request_handler_test tests/request_handler_tests/proxy_request_handler_test.cc)
target_link_libraries(proxy_request_handler_test request_handler_lib http_response http_request gtest_main Boost::system Boost::log Boost::regex)

add_executable(request_manager_test tests/request_handler_tests/request_manager_test.cc)
target_link_libraries(request_manager_test request_handler_lib config_parser_lib http_response http_request gtest_main Boost::system Boost::log Boost::regex)

add_executable(dispatcher_test tests/dispatcher_tests/dispatcher_test.cc)
target_link_libraries(dispatcher_test request_handler_lib config_parser_lib http_response http_request gtest_main Boost::system Boost::log Boost::regex)

add_executable(http_request_test tests/http_request_object_tests/http_request_test.cc)
target_link_libraries(http_request_test http_request gtest_main Boost::system Boost::log Boost::regex)

# TODO(!): Update with tests binary
gtest_discover_tests(config_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/config_parser_tests)
gtest_discover_tests(session_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/server_tests)
gtest_discover_tests(server_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/server_tests)
gtest_discover_tests(static_file_request_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/request_handler_tests)
gtest_discover_tests(action_request_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/request_handler_tests)
gtest_discover_tests(proxy_request_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/request_handler_tests)
gtest_discover_tests(request_manager_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/request_handler_tests)
gtest_discover_tests(dispatcher_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/dispatcher_tests)
gtest_discover_tests(http_request_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/http_request_object_tests)

 add_test(NAME server_integration_test
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/shell_tests/integration_test.sh "./test_config"
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shell_tests)

add_test(NAME threadedness_test
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/shell_tests/test_threads.sh "./test_config"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shell_tests)

# TODO(!): Update with target/tests targets
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    include(cmake/CodeCoverageReportConfig.cmake)
    generate_coverage_report(TARGETS server server_lib config_parser_lib request_handler_lib TESTS config_parser_test session_test server_test static_file_request_handler_test action_request_handler_test proxy_request_handler_test request_manager_test dispatcher_test http_request_test)
endif()
