cmake_minimum_required(VERSION 3.10.0)
project(rubberduck_server)

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
set(CMAKE_BUILD_TYPE Debug)

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)

# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system filesystem log regex)
message(STATUS "Boost version: ${Boost_VERSION}")

include_directories(include)

# Locate bash program
find_program (BASH_PROGRAM bash)


# TODO(!): Update name and srcs
add_library(logger src/logger.cc)
add_library(session src/session.cc src/server.cc)
add_library(server_c src/server.cc src/session.cc)
add_library(config_parser src/config_parser.cc)
add_library(request_parser src/http/request_parser.cc src/http/reply.cc)
add_library(reply src/http/reply.cc)
add_library(request_handler
            src/request_handler_dispatcher.cc
            src/request_handler/request_handler_static.cc
            src/request_handler/request_handler_echo.cc
            src/request_handler/request_handler_error.cc
            src/request_handler/request_handler_status.cc
            src/http/mime_types.cc)


# TODO(!): Update executable name, srcs, and deps
add_executable(server src/server_main.cc)
target_link_libraries(server server_c session request_handler request_parser logger
                      config_parser reply Boost::system Boost::filesystem
                      Boost::regex Boost::log_setup Boost::log)

target_link_libraries(session logger Boost::regex Boost::log_setup Boost::log Boost::system Boost::filesystem)

# TODO(!): Update test executable name, srcs, and deps
add_executable(config_parser_test tests/config_parser_test.cc)
add_executable(request_parser_test tests/request_parser_test.cc)
add_executable(reply_test tests/reply_test.cc)
add_executable(session_test tests/session_test.cc)
add_executable(logger_test tests/logger_test.cc)
add_executable(request_handler_test tests/request_handler_test.cc)
target_link_libraries(config_parser_test config_parser gtest_main)
target_link_libraries(request_parser_test request_parser gtest_main)
target_link_libraries(logger_test logger gtest_main Boost::log)
target_link_libraries(request_handler_test request_handler session gtest_main request_parser config_parser reply)
target_link_libraries(reply_test reply gtest_main Boost::system)
target_link_libraries(session_test session request_handler request_parser
                      gtest_main gmock_main)




# TODO(!): Update with test binary
gtest_discover_tests(config_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(reply_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(session_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(logger_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_test(NAME integration_test COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.sh)
add_test(NAME integration_test_concurrency COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_concurrency.sh)


# TODO(!): Update with target/test targets
include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(TARGETS session server config_parser request_parser
                         reply logger request_handler TESTS config_parser_test request_parser_test request_handler_test
                         reply_test session_test logger_test)
