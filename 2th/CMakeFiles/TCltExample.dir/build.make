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
include CMakeFiles/TCltExample.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TCltExample.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TCltExample.dir/flags.make

CMakeFiles/TCltExample.dir/CltExample.c.o: CMakeFiles/TCltExample.dir/flags.make
CMakeFiles/TCltExample.dir/CltExample.c.o: CltExample.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/oshare/work/2th/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/TCltExample.dir/CltExample.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/TCltExample.dir/CltExample.c.o   -c /home/oshare/work/2th/CltExample.c

CMakeFiles/TCltExample.dir/CltExample.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/TCltExample.dir/CltExample.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/oshare/work/2th/CltExample.c > CMakeFiles/TCltExample.dir/CltExample.c.i

CMakeFiles/TCltExample.dir/CltExample.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/TCltExample.dir/CltExample.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/oshare/work/2th/CltExample.c -o CMakeFiles/TCltExample.dir/CltExample.c.s

# Object files for target TCltExample
TCltExample_OBJECTS = \
"CMakeFiles/TCltExample.dir/CltExample.c.o"

# External object files for target TCltExample
TCltExample_EXTERNAL_OBJECTS =

TCltExample: CMakeFiles/TCltExample.dir/CltExample.c.o
TCltExample: CMakeFiles/TCltExample.dir/build.make
TCltExample: libExampleClientlib.so
TCltExample: libExampleServerlib.a
TCltExample: CMakeFiles/TCltExample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/oshare/work/2th/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable TCltExample"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TCltExample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TCltExample.dir/build: TCltExample

.PHONY : CMakeFiles/TCltExample.dir/build

CMakeFiles/TCltExample.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TCltExample.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TCltExample.dir/clean

CMakeFiles/TCltExample.dir/depend:
	cd /home/oshare/work/2th && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oshare/work/2th /home/oshare/work/2th /home/oshare/work/2th /home/oshare/work/2th /home/oshare/work/2th/CMakeFiles/TCltExample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TCltExample.dir/depend
