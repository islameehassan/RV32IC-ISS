# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/islam/AUC/junior/Assembly/Project/RV32IC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/islam/AUC/junior/Assembly/Project/RV32IC/build

# Include any dependencies generated for this target.
include CMakeFiles/RV32IC_ISS.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RV32IC_ISS.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RV32IC_ISS.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RV32IC_ISS.dir/flags.make

CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.o: CMakeFiles/RV32IC_ISS.dir/flags.make
CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.o: ../lib/disassembler.cpp
CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.o: CMakeFiles/RV32IC_ISS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/islam/AUC/junior/Assembly/Project/RV32IC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.o -MF CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.o.d -o CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.o -c /home/islam/AUC/junior/Assembly/Project/RV32IC/lib/disassembler.cpp

CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/islam/AUC/junior/Assembly/Project/RV32IC/lib/disassembler.cpp > CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.i

CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/islam/AUC/junior/Assembly/Project/RV32IC/lib/disassembler.cpp -o CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.s

CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.o: CMakeFiles/RV32IC_ISS.dir/flags.make
CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.o: ../lib/memory.cpp
CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.o: CMakeFiles/RV32IC_ISS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/islam/AUC/junior/Assembly/Project/RV32IC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.o -MF CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.o.d -o CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.o -c /home/islam/AUC/junior/Assembly/Project/RV32IC/lib/memory.cpp

CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/islam/AUC/junior/Assembly/Project/RV32IC/lib/memory.cpp > CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.i

CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/islam/AUC/junior/Assembly/Project/RV32IC/lib/memory.cpp -o CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.s

CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.o: CMakeFiles/RV32IC_ISS.dir/flags.make
CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.o: ../lib/registerfile.cpp
CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.o: CMakeFiles/RV32IC_ISS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/islam/AUC/junior/Assembly/Project/RV32IC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.o -MF CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.o.d -o CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.o -c /home/islam/AUC/junior/Assembly/Project/RV32IC/lib/registerfile.cpp

CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/islam/AUC/junior/Assembly/Project/RV32IC/lib/registerfile.cpp > CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.i

CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/islam/AUC/junior/Assembly/Project/RV32IC/lib/registerfile.cpp -o CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.s

CMakeFiles/RV32IC_ISS.dir/main.cpp.o: CMakeFiles/RV32IC_ISS.dir/flags.make
CMakeFiles/RV32IC_ISS.dir/main.cpp.o: ../main.cpp
CMakeFiles/RV32IC_ISS.dir/main.cpp.o: CMakeFiles/RV32IC_ISS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/islam/AUC/junior/Assembly/Project/RV32IC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/RV32IC_ISS.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RV32IC_ISS.dir/main.cpp.o -MF CMakeFiles/RV32IC_ISS.dir/main.cpp.o.d -o CMakeFiles/RV32IC_ISS.dir/main.cpp.o -c /home/islam/AUC/junior/Assembly/Project/RV32IC/main.cpp

CMakeFiles/RV32IC_ISS.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RV32IC_ISS.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/islam/AUC/junior/Assembly/Project/RV32IC/main.cpp > CMakeFiles/RV32IC_ISS.dir/main.cpp.i

CMakeFiles/RV32IC_ISS.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RV32IC_ISS.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/islam/AUC/junior/Assembly/Project/RV32IC/main.cpp -o CMakeFiles/RV32IC_ISS.dir/main.cpp.s

# Object files for target RV32IC_ISS
RV32IC_ISS_OBJECTS = \
"CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.o" \
"CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.o" \
"CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.o" \
"CMakeFiles/RV32IC_ISS.dir/main.cpp.o"

# External object files for target RV32IC_ISS
RV32IC_ISS_EXTERNAL_OBJECTS =

RV32IC_ISS: CMakeFiles/RV32IC_ISS.dir/lib/disassembler.cpp.o
RV32IC_ISS: CMakeFiles/RV32IC_ISS.dir/lib/memory.cpp.o
RV32IC_ISS: CMakeFiles/RV32IC_ISS.dir/lib/registerfile.cpp.o
RV32IC_ISS: CMakeFiles/RV32IC_ISS.dir/main.cpp.o
RV32IC_ISS: CMakeFiles/RV32IC_ISS.dir/build.make
RV32IC_ISS: CMakeFiles/RV32IC_ISS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/islam/AUC/junior/Assembly/Project/RV32IC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable RV32IC_ISS"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RV32IC_ISS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RV32IC_ISS.dir/build: RV32IC_ISS
.PHONY : CMakeFiles/RV32IC_ISS.dir/build

CMakeFiles/RV32IC_ISS.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RV32IC_ISS.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RV32IC_ISS.dir/clean

CMakeFiles/RV32IC_ISS.dir/depend:
	cd /home/islam/AUC/junior/Assembly/Project/RV32IC/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/islam/AUC/junior/Assembly/Project/RV32IC /home/islam/AUC/junior/Assembly/Project/RV32IC /home/islam/AUC/junior/Assembly/Project/RV32IC/build /home/islam/AUC/junior/Assembly/Project/RV32IC/build /home/islam/AUC/junior/Assembly/Project/RV32IC/build/CMakeFiles/RV32IC_ISS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RV32IC_ISS.dir/depend

