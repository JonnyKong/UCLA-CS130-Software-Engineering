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
include CMakeFiles/request_handler.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/request_handler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/request_handler.dir/flags.make

CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o: CMakeFiles/request_handler.dir/flags.make
CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o: ../src/request_handler_dispatcher.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/usr/src/projects/rubberduck/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o -c /usr/src/projects/rubberduck/src/request_handler_dispatcher.cc

CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /usr/src/projects/rubberduck/src/request_handler_dispatcher.cc > CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.i

CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /usr/src/projects/rubberduck/src/request_handler_dispatcher.cc -o CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.s

CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o.requires:

.PHONY : CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o.requires

CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o.provides: CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o.requires
	$(MAKE) -f CMakeFiles/request_handler.dir/build.make CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o.provides.build
.PHONY : CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o.provides

CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o.provides.build: CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o


CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o: CMakeFiles/request_handler.dir/flags.make
CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o: ../src/request_handler/request_handler_static.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/usr/src/projects/rubberduck/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o -c /usr/src/projects/rubberduck/src/request_handler/request_handler_static.cc

CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /usr/src/projects/rubberduck/src/request_handler/request_handler_static.cc > CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.i

CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /usr/src/projects/rubberduck/src/request_handler/request_handler_static.cc -o CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.s

CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o.requires:

.PHONY : CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o.requires

CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o.provides: CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o.requires
	$(MAKE) -f CMakeFiles/request_handler.dir/build.make CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o.provides.build
.PHONY : CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o.provides

CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o.provides.build: CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o


CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o: CMakeFiles/request_handler.dir/flags.make
CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o: ../src/request_handler/request_handler_echo.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/usr/src/projects/rubberduck/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o -c /usr/src/projects/rubberduck/src/request_handler/request_handler_echo.cc

CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /usr/src/projects/rubberduck/src/request_handler/request_handler_echo.cc > CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.i

CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /usr/src/projects/rubberduck/src/request_handler/request_handler_echo.cc -o CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.s

CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o.requires:

.PHONY : CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o.requires

CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o.provides: CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o.requires
	$(MAKE) -f CMakeFiles/request_handler.dir/build.make CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o.provides.build
.PHONY : CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o.provides

CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o.provides.build: CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o


CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o: CMakeFiles/request_handler.dir/flags.make
CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o: ../src/request_handler/request_handler_error.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/usr/src/projects/rubberduck/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o -c /usr/src/projects/rubberduck/src/request_handler/request_handler_error.cc

CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /usr/src/projects/rubberduck/src/request_handler/request_handler_error.cc > CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.i

CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /usr/src/projects/rubberduck/src/request_handler/request_handler_error.cc -o CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.s

CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o.requires:

.PHONY : CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o.requires

CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o.provides: CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o.requires
	$(MAKE) -f CMakeFiles/request_handler.dir/build.make CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o.provides.build
.PHONY : CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o.provides

CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o.provides.build: CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o


CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o: CMakeFiles/request_handler.dir/flags.make
CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o: ../src/request_handler/request_handler_status.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/usr/src/projects/rubberduck/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o -c /usr/src/projects/rubberduck/src/request_handler/request_handler_status.cc

CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /usr/src/projects/rubberduck/src/request_handler/request_handler_status.cc > CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.i

CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /usr/src/projects/rubberduck/src/request_handler/request_handler_status.cc -o CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.s

CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o.requires:

.PHONY : CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o.requires

CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o.provides: CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o.requires
	$(MAKE) -f CMakeFiles/request_handler.dir/build.make CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o.provides.build
.PHONY : CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o.provides

CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o.provides.build: CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o


CMakeFiles/request_handler.dir/src/http/mime_types.cc.o: CMakeFiles/request_handler.dir/flags.make
CMakeFiles/request_handler.dir/src/http/mime_types.cc.o: ../src/http/mime_types.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/usr/src/projects/rubberduck/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/request_handler.dir/src/http/mime_types.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/request_handler.dir/src/http/mime_types.cc.o -c /usr/src/projects/rubberduck/src/http/mime_types.cc

CMakeFiles/request_handler.dir/src/http/mime_types.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/request_handler.dir/src/http/mime_types.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /usr/src/projects/rubberduck/src/http/mime_types.cc > CMakeFiles/request_handler.dir/src/http/mime_types.cc.i

CMakeFiles/request_handler.dir/src/http/mime_types.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/request_handler.dir/src/http/mime_types.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /usr/src/projects/rubberduck/src/http/mime_types.cc -o CMakeFiles/request_handler.dir/src/http/mime_types.cc.s

CMakeFiles/request_handler.dir/src/http/mime_types.cc.o.requires:

.PHONY : CMakeFiles/request_handler.dir/src/http/mime_types.cc.o.requires

CMakeFiles/request_handler.dir/src/http/mime_types.cc.o.provides: CMakeFiles/request_handler.dir/src/http/mime_types.cc.o.requires
	$(MAKE) -f CMakeFiles/request_handler.dir/build.make CMakeFiles/request_handler.dir/src/http/mime_types.cc.o.provides.build
.PHONY : CMakeFiles/request_handler.dir/src/http/mime_types.cc.o.provides

CMakeFiles/request_handler.dir/src/http/mime_types.cc.o.provides.build: CMakeFiles/request_handler.dir/src/http/mime_types.cc.o


# Object files for target request_handler
request_handler_OBJECTS = \
"CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o" \
"CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o" \
"CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o" \
"CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o" \
"CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o" \
"CMakeFiles/request_handler.dir/src/http/mime_types.cc.o"

# External object files for target request_handler
request_handler_EXTERNAL_OBJECTS =

librequest_handler.a: CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o
librequest_handler.a: CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o
librequest_handler.a: CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o
librequest_handler.a: CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o
librequest_handler.a: CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o
librequest_handler.a: CMakeFiles/request_handler.dir/src/http/mime_types.cc.o
librequest_handler.a: CMakeFiles/request_handler.dir/build.make
librequest_handler.a: CMakeFiles/request_handler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/usr/src/projects/rubberduck/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX static library librequest_handler.a"
	$(CMAKE_COMMAND) -P CMakeFiles/request_handler.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/request_handler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/request_handler.dir/build: librequest_handler.a

.PHONY : CMakeFiles/request_handler.dir/build

CMakeFiles/request_handler.dir/requires: CMakeFiles/request_handler.dir/src/request_handler_dispatcher.cc.o.requires
CMakeFiles/request_handler.dir/requires: CMakeFiles/request_handler.dir/src/request_handler/request_handler_static.cc.o.requires
CMakeFiles/request_handler.dir/requires: CMakeFiles/request_handler.dir/src/request_handler/request_handler_echo.cc.o.requires
CMakeFiles/request_handler.dir/requires: CMakeFiles/request_handler.dir/src/request_handler/request_handler_error.cc.o.requires
CMakeFiles/request_handler.dir/requires: CMakeFiles/request_handler.dir/src/request_handler/request_handler_status.cc.o.requires
CMakeFiles/request_handler.dir/requires: CMakeFiles/request_handler.dir/src/http/mime_types.cc.o.requires

.PHONY : CMakeFiles/request_handler.dir/requires

CMakeFiles/request_handler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/request_handler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/request_handler.dir/clean

CMakeFiles/request_handler.dir/depend:
	cd /usr/src/projects/rubberduck/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /usr/src/projects/rubberduck /usr/src/projects/rubberduck /usr/src/projects/rubberduck/build /usr/src/projects/rubberduck/build /usr/src/projects/rubberduck/build/CMakeFiles/request_handler.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/request_handler.dir/depend

