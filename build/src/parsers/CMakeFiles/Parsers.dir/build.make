# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.18

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\jonme\ALL_PROJECTS\yuml

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\jonme\ALL_PROJECTS\yuml\build

# Include any dependencies generated for this target.
include src/parsers/CMakeFiles/Parsers.dir/depend.make

# Include the progress variables for this target.
include src/parsers/CMakeFiles/Parsers.dir/progress.make

# Include the compile flags for this target's objects.
include src/parsers/CMakeFiles/Parsers.dir/flags.make

src/parsers/CMakeFiles/Parsers.dir/inputParser.cpp.obj: src/parsers/CMakeFiles/Parsers.dir/flags.make
src/parsers/CMakeFiles/Parsers.dir/inputParser.cpp.obj: src/parsers/CMakeFiles/Parsers.dir/includes_CXX.rsp
src/parsers/CMakeFiles/Parsers.dir/inputParser.cpp.obj: ../src/parsers/inputParser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/parsers/CMakeFiles/Parsers.dir/inputParser.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Parsers.dir\inputParser.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\inputParser.cpp

src/parsers/CMakeFiles/Parsers.dir/inputParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Parsers.dir/inputParser.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\inputParser.cpp > CMakeFiles\Parsers.dir\inputParser.cpp.i

src/parsers/CMakeFiles/Parsers.dir/inputParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Parsers.dir/inputParser.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\inputParser.cpp -o CMakeFiles\Parsers.dir\inputParser.cpp.s

src/parsers/CMakeFiles/Parsers.dir/templateParser.cpp.obj: src/parsers/CMakeFiles/Parsers.dir/flags.make
src/parsers/CMakeFiles/Parsers.dir/templateParser.cpp.obj: src/parsers/CMakeFiles/Parsers.dir/includes_CXX.rsp
src/parsers/CMakeFiles/Parsers.dir/templateParser.cpp.obj: ../src/parsers/templateParser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/parsers/CMakeFiles/Parsers.dir/templateParser.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Parsers.dir\templateParser.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\templateParser.cpp

src/parsers/CMakeFiles/Parsers.dir/templateParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Parsers.dir/templateParser.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\templateParser.cpp > CMakeFiles\Parsers.dir\templateParser.cpp.i

src/parsers/CMakeFiles/Parsers.dir/templateParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Parsers.dir/templateParser.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\templateParser.cpp -o CMakeFiles\Parsers.dir\templateParser.cpp.s

src/parsers/CMakeFiles/Parsers.dir/namespaceParser.cpp.obj: src/parsers/CMakeFiles/Parsers.dir/flags.make
src/parsers/CMakeFiles/Parsers.dir/namespaceParser.cpp.obj: src/parsers/CMakeFiles/Parsers.dir/includes_CXX.rsp
src/parsers/CMakeFiles/Parsers.dir/namespaceParser.cpp.obj: ../src/parsers/namespaceParser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/parsers/CMakeFiles/Parsers.dir/namespaceParser.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Parsers.dir\namespaceParser.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\namespaceParser.cpp

src/parsers/CMakeFiles/Parsers.dir/namespaceParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Parsers.dir/namespaceParser.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\namespaceParser.cpp > CMakeFiles\Parsers.dir\namespaceParser.cpp.i

src/parsers/CMakeFiles/Parsers.dir/namespaceParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Parsers.dir/namespaceParser.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\namespaceParser.cpp -o CMakeFiles\Parsers.dir\namespaceParser.cpp.s

src/parsers/CMakeFiles/Parsers.dir/modelParser.cpp.obj: src/parsers/CMakeFiles/Parsers.dir/flags.make
src/parsers/CMakeFiles/Parsers.dir/modelParser.cpp.obj: src/parsers/CMakeFiles/Parsers.dir/includes_CXX.rsp
src/parsers/CMakeFiles/Parsers.dir/modelParser.cpp.obj: ../src/parsers/modelParser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/parsers/CMakeFiles/Parsers.dir/modelParser.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Parsers.dir\modelParser.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\modelParser.cpp

src/parsers/CMakeFiles/Parsers.dir/modelParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Parsers.dir/modelParser.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\modelParser.cpp > CMakeFiles\Parsers.dir\modelParser.cpp.i

src/parsers/CMakeFiles/Parsers.dir/modelParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Parsers.dir/modelParser.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\modelParser.cpp -o CMakeFiles\Parsers.dir\modelParser.cpp.s

src/parsers/CMakeFiles/Parsers.dir/elementParser.cpp.obj: src/parsers/CMakeFiles/Parsers.dir/flags.make
src/parsers/CMakeFiles/Parsers.dir/elementParser.cpp.obj: src/parsers/CMakeFiles/Parsers.dir/includes_CXX.rsp
src/parsers/CMakeFiles/Parsers.dir/elementParser.cpp.obj: ../src/parsers/elementParser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/parsers/CMakeFiles/Parsers.dir/elementParser.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Parsers.dir\elementParser.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\elementParser.cpp

src/parsers/CMakeFiles/Parsers.dir/elementParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Parsers.dir/elementParser.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\elementParser.cpp > CMakeFiles\Parsers.dir\elementParser.cpp.i

src/parsers/CMakeFiles/Parsers.dir/elementParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Parsers.dir/elementParser.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\elementParser.cpp -o CMakeFiles\Parsers.dir\elementParser.cpp.s

src/parsers/CMakeFiles/Parsers.dir/namedElementParser.cpp.obj: src/parsers/CMakeFiles/Parsers.dir/flags.make
src/parsers/CMakeFiles/Parsers.dir/namedElementParser.cpp.obj: src/parsers/CMakeFiles/Parsers.dir/includes_CXX.rsp
src/parsers/CMakeFiles/Parsers.dir/namedElementParser.cpp.obj: ../src/parsers/namedElementParser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/parsers/CMakeFiles/Parsers.dir/namedElementParser.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Parsers.dir\namedElementParser.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\namedElementParser.cpp

src/parsers/CMakeFiles/Parsers.dir/namedElementParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Parsers.dir/namedElementParser.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\namedElementParser.cpp > CMakeFiles\Parsers.dir\namedElementParser.cpp.i

src/parsers/CMakeFiles/Parsers.dir/namedElementParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Parsers.dir/namedElementParser.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers\namedElementParser.cpp -o CMakeFiles\Parsers.dir\namedElementParser.cpp.s

# Object files for target Parsers
Parsers_OBJECTS = \
"CMakeFiles/Parsers.dir/inputParser.cpp.obj" \
"CMakeFiles/Parsers.dir/templateParser.cpp.obj" \
"CMakeFiles/Parsers.dir/namespaceParser.cpp.obj" \
"CMakeFiles/Parsers.dir/modelParser.cpp.obj" \
"CMakeFiles/Parsers.dir/elementParser.cpp.obj" \
"CMakeFiles/Parsers.dir/namedElementParser.cpp.obj"

# External object files for target Parsers
Parsers_EXTERNAL_OBJECTS =

src/parsers/libParsers.a: src/parsers/CMakeFiles/Parsers.dir/inputParser.cpp.obj
src/parsers/libParsers.a: src/parsers/CMakeFiles/Parsers.dir/templateParser.cpp.obj
src/parsers/libParsers.a: src/parsers/CMakeFiles/Parsers.dir/namespaceParser.cpp.obj
src/parsers/libParsers.a: src/parsers/CMakeFiles/Parsers.dir/modelParser.cpp.obj
src/parsers/libParsers.a: src/parsers/CMakeFiles/Parsers.dir/elementParser.cpp.obj
src/parsers/libParsers.a: src/parsers/CMakeFiles/Parsers.dir/namedElementParser.cpp.obj
src/parsers/libParsers.a: src/parsers/CMakeFiles/Parsers.dir/build.make
src/parsers/libParsers.a: src/parsers/CMakeFiles/Parsers.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX static library libParsers.a"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && $(CMAKE_COMMAND) -P CMakeFiles\Parsers.dir\cmake_clean_target.cmake
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Parsers.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/parsers/CMakeFiles/Parsers.dir/build: src/parsers/libParsers.a

.PHONY : src/parsers/CMakeFiles/Parsers.dir/build

src/parsers/CMakeFiles/Parsers.dir/clean:
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers && $(CMAKE_COMMAND) -P CMakeFiles\Parsers.dir\cmake_clean.cmake
.PHONY : src/parsers/CMakeFiles/Parsers.dir/clean

src/parsers/CMakeFiles/Parsers.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\jonme\ALL_PROJECTS\yuml C:\Users\jonme\ALL_PROJECTS\yuml\src\parsers C:\Users\jonme\ALL_PROJECTS\yuml\build C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers C:\Users\jonme\ALL_PROJECTS\yuml\build\src\parsers\CMakeFiles\Parsers.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src/parsers/CMakeFiles/Parsers.dir/depend

