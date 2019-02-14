# CMake generated Testfile for 
# Source directory: /usr/src/projects/rubberduck
# Build directory: /usr/src/projects/rubberduck/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("/usr/src/projects/rubberduck/build/config_parser_test[1]_include.cmake")
include("/usr/src/projects/rubberduck/build/request_parser_test[1]_include.cmake")
include("/usr/src/projects/rubberduck/build/session_test[1]_include.cmake")
include("/usr/src/projects/rubberduck/build/logger_test[1]_include.cmake")
add_test(integration_test "/bin/bash" "/usr/src/projects/rubberduck/tests/test.sh")
subdirs("googletest")
