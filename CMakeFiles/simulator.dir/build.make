# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/csc3050/CSC3050_P2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/csc3050/CSC3050_P2

# Include any dependencies generated for this target.
include CMakeFiles/simulator.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/simulator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/simulator.dir/flags.make

CMakeFiles/simulator.dir/simulator.cpp.o: CMakeFiles/simulator.dir/flags.make
CMakeFiles/simulator.dir/simulator.cpp.o: simulator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/csc3050/CSC3050_P2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/simulator.dir/simulator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simulator.dir/simulator.cpp.o -c /home/csc3050/CSC3050_P2/simulator.cpp

CMakeFiles/simulator.dir/simulator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simulator.dir/simulator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/csc3050/CSC3050_P2/simulator.cpp > CMakeFiles/simulator.dir/simulator.cpp.i

CMakeFiles/simulator.dir/simulator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simulator.dir/simulator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/csc3050/CSC3050_P2/simulator.cpp -o CMakeFiles/simulator.dir/simulator.cpp.s

CMakeFiles/simulator.dir/phase1.cpp.o: CMakeFiles/simulator.dir/flags.make
CMakeFiles/simulator.dir/phase1.cpp.o: phase1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/csc3050/CSC3050_P2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/simulator.dir/phase1.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simulator.dir/phase1.cpp.o -c /home/csc3050/CSC3050_P2/phase1.cpp

CMakeFiles/simulator.dir/phase1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simulator.dir/phase1.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/csc3050/CSC3050_P2/phase1.cpp > CMakeFiles/simulator.dir/phase1.cpp.i

CMakeFiles/simulator.dir/phase1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simulator.dir/phase1.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/csc3050/CSC3050_P2/phase1.cpp -o CMakeFiles/simulator.dir/phase1.cpp.s

CMakeFiles/simulator.dir/phase2.cpp.o: CMakeFiles/simulator.dir/flags.make
CMakeFiles/simulator.dir/phase2.cpp.o: phase2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/csc3050/CSC3050_P2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/simulator.dir/phase2.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simulator.dir/phase2.cpp.o -c /home/csc3050/CSC3050_P2/phase2.cpp

CMakeFiles/simulator.dir/phase2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simulator.dir/phase2.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/csc3050/CSC3050_P2/phase2.cpp > CMakeFiles/simulator.dir/phase2.cpp.i

CMakeFiles/simulator.dir/phase2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simulator.dir/phase2.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/csc3050/CSC3050_P2/phase2.cpp -o CMakeFiles/simulator.dir/phase2.cpp.s

# Object files for target simulator
simulator_OBJECTS = \
"CMakeFiles/simulator.dir/simulator.cpp.o" \
"CMakeFiles/simulator.dir/phase1.cpp.o" \
"CMakeFiles/simulator.dir/phase2.cpp.o"

# External object files for target simulator
simulator_EXTERNAL_OBJECTS =

simulator: CMakeFiles/simulator.dir/simulator.cpp.o
simulator: CMakeFiles/simulator.dir/phase1.cpp.o
simulator: CMakeFiles/simulator.dir/phase2.cpp.o
simulator: CMakeFiles/simulator.dir/build.make
simulator: CMakeFiles/simulator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/csc3050/CSC3050_P2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable simulator"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simulator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/simulator.dir/build: simulator

.PHONY : CMakeFiles/simulator.dir/build

CMakeFiles/simulator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/simulator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/simulator.dir/clean

CMakeFiles/simulator.dir/depend:
	cd /home/csc3050/CSC3050_P2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/csc3050/CSC3050_P2 /home/csc3050/CSC3050_P2 /home/csc3050/CSC3050_P2 /home/csc3050/CSC3050_P2 /home/csc3050/CSC3050_P2/CMakeFiles/simulator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/simulator.dir/depend

