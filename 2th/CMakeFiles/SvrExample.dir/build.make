# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/oshare/work/2th

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oshare/work/2th

# Include any dependencies generated for this target.
include CMakeFiles/SvrExample.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SvrExample.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SvrExample.dir/flags.make

CMakeFiles/SvrExample.dir/SvrExample.c.o: CMakeFiles/SvrExample.dir/flags.make
CMakeFiles/SvrExample.dir/SvrExample.c.o: SvrExample.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/oshare/work/2th/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/SvrExample.dir/SvrExample.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/SvrExample.dir/SvrExample.c.o   -c /home/oshare/work/2th/SvrExample.c

CMakeFiles/SvrExample.dir/SvrExample.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SvrExample.dir/SvrExample.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/oshare/work/2th/SvrExample.c > CMakeFiles/SvrExample.dir/SvrExample.c.i

CMakeFiles/SvrExample.dir/SvrExample.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SvrExample.dir/SvrExample.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/oshare/work/2th/SvrExample.c -o CMakeFiles/SvrExample.dir/SvrExample.c.s

# Object files for target SvrExample
SvrExample_OBJECTS = \
"CMakeFiles/SvrExample.dir/SvrExample.c.o"

# External object files for target SvrExample
SvrExample_EXTERNAL_OBJECTS =

SvrExample: CMakeFiles/SvrExample.dir/SvrExample.c.o
SvrExample: CMakeFiles/SvrExample.dir/build.make
SvrExample: libExampleServerlib.a
SvrExample: CMakeFiles/SvrExample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/oshare/work/2th/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable SvrExample"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SvrExample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SvrExample.dir/build: SvrExample

.PHONY : CMakeFiles/SvrExample.dir/build

CMakeFiles/SvrExample.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SvrExample.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SvrExample.dir/clean

CMakeFiles/SvrExample.dir/depend:
	cd /home/oshare/work/2th && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oshare/work/2th /home/oshare/work/2th /home/oshare/work/2th /home/oshare/work/2th /home/oshare/work/2th/CMakeFiles/SvrExample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SvrExample.dir/depend
