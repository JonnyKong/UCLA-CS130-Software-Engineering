# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /usr/src/projects/rubberduck

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /usr/src/projects/rubberduck/build

# Include any dependencies generated for this target.
include CMakeFiles/reply_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/reply_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/reply_test.dir/flags.make

CMakeFiles/reply_test.dir/tests/reply_test.cc.o: CMakeFiles/reply_test.dir/flags.make
CMakeFiles/reply_test.dir/tests/reply_test.cc.o: ../tests/reply_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/usr/src/projects/rubberduck/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/reply_test.dir/tests/reply_test.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/reply_test.dir/tests/reply_test.cc.o -c /usr/src/projects/rubberduck/tests/reply_test.cc

CMakeFiles/reply_test.dir/tests/reply_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reply_test.dir/tests/reply_test.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /usr/src/projects/rubberduck/tests/reply_test.cc > CMakeFiles/reply_test.dir/tests/reply_test.cc.i

CMakeFiles/reply_test.dir/tests/reply_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reply_test.dir/tests/reply_test.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /usr/src/projects/rubberduck/tests/reply_test.cc -o CMakeFiles/reply_test.dir/tests/reply_test.cc.s

CMakeFiles/reply_test.dir/tests/reply_test.cc.o.requires:

.PHONY : CMakeFiles/reply_test.dir/tests/reply_test.cc.o.requires

CMakeFiles/reply_test.dir/tests/reply_test.cc.o.provides: CMakeFiles/reply_test.dir/tests/reply_test.cc.o.requires
	$(MAKE) -f CMakeFiles/reply_test.dir/build.make CMakeFiles/reply_test.dir/tests/reply_test.cc.o.provides.build
.PHONY : CMakeFiles/reply_test.dir/tests/reply_test.cc.o.provides

CMakeFiles/reply_test.dir/tests/reply_test.cc.o.provides.build: CMakeFiles/reply_test.dir/tests/reply_test.cc.o


# Object files for target reply_test
reply_test_OBJECTS = \
"CMakeFiles/reply_test.dir/tests/reply_test.cc.o"

# External object files for target reply_test
reply_test_EXTERNAL_OBJECTS =

bin/reply_test: CMakeFiles/reply_test.dir/tests/reply_test.cc.o
bin/reply_test: CMakeFiles/reply_test.dir/build.make
bin/reply_test: libreply.a
bin/reply_test: googletest/googlemock/gtest/libgtest_main.a
bin/reply_test: /usr/lib/x86_64-linux-gnu/libboost_system.a
bin/reply_test: googletest/googlemock/gtest/libgtest.a
bin/reply_test: CMakeFiles/reply_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/usr/src/projects/rubberduck/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/reply_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/reply_test.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -D TEST_TARGET=reply_test -D TEST_EXECUTABLE=/usr/src/projects/rubberduck/build/bin/reply_test -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/usr/src/projects/rubberduck/tests -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=reply_test_TESTS -D CTEST_FILE=/usr/src/projects/rubberduck/build/reply_test[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -P /usr/share/cmake-3.10/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
CMakeFiles/reply_test.dir/build: bin/reply_test

.PHONY : CMakeFiles/reply_test.dir/build

CMakeFiles/reply_test.dir/requires: CMakeFiles/reply_test.dir/tests/reply_test.cc.o.requires

.PHONY : CMakeFiles/reply_test.dir/requires

CMakeFiles/reply_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/reply_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/reply_test.dir/clean

CMakeFiles/reply_test.dir/depend:
	cd /usr/src/projects/rubberduck/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /usr/src/projects/rubberduck /usr/src/projects/rubberduck /usr/src/projects/rubberduck/build /usr/src/projects/rubberduck/build /usr/src/projects/rubberduck/build/CMakeFiles/reply_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/reply_test.dir/depend
