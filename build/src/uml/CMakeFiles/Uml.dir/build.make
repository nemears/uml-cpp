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
include src/uml/CMakeFiles/Uml.dir/depend.make

# Include the progress variables for this target.
include src/uml/CMakeFiles/Uml.dir/progress.make

# Include the compile flags for this target's objects.
include src/uml/CMakeFiles/Uml.dir/flags.make

src/uml/CMakeFiles/Uml.dir/namespace.cpp.obj: src/uml/CMakeFiles/Uml.dir/flags.make
src/uml/CMakeFiles/Uml.dir/namespace.cpp.obj: src/uml/CMakeFiles/Uml.dir/includes_CXX.rsp
src/uml/CMakeFiles/Uml.dir/namespace.cpp.obj: ../src/uml/namespace.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/uml/CMakeFiles/Uml.dir/namespace.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Uml.dir\namespace.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\namespace.cpp

src/uml/CMakeFiles/Uml.dir/namespace.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Uml.dir/namespace.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\namespace.cpp > CMakeFiles\Uml.dir\namespace.cpp.i

src/uml/CMakeFiles/Uml.dir/namespace.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Uml.dir/namespace.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\namespace.cpp -o CMakeFiles\Uml.dir\namespace.cpp.s

src/uml/CMakeFiles/Uml.dir/model.cpp.obj: src/uml/CMakeFiles/Uml.dir/flags.make
src/uml/CMakeFiles/Uml.dir/model.cpp.obj: src/uml/CMakeFiles/Uml.dir/includes_CXX.rsp
src/uml/CMakeFiles/Uml.dir/model.cpp.obj: ../src/uml/model.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/uml/CMakeFiles/Uml.dir/model.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Uml.dir\model.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\model.cpp

src/uml/CMakeFiles/Uml.dir/model.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Uml.dir/model.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\model.cpp > CMakeFiles\Uml.dir\model.cpp.i

src/uml/CMakeFiles/Uml.dir/model.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Uml.dir/model.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\model.cpp -o CMakeFiles\Uml.dir\model.cpp.s

src/uml/CMakeFiles/Uml.dir/element.cpp.obj: src/uml/CMakeFiles/Uml.dir/flags.make
src/uml/CMakeFiles/Uml.dir/element.cpp.obj: src/uml/CMakeFiles/Uml.dir/includes_CXX.rsp
src/uml/CMakeFiles/Uml.dir/element.cpp.obj: ../src/uml/element.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/uml/CMakeFiles/Uml.dir/element.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Uml.dir\element.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\element.cpp

src/uml/CMakeFiles/Uml.dir/element.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Uml.dir/element.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\element.cpp > CMakeFiles\Uml.dir\element.cpp.i

src/uml/CMakeFiles/Uml.dir/element.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Uml.dir/element.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\element.cpp -o CMakeFiles\Uml.dir\element.cpp.s

src/uml/CMakeFiles/Uml.dir/namedElement.cpp.obj: src/uml/CMakeFiles/Uml.dir/flags.make
src/uml/CMakeFiles/Uml.dir/namedElement.cpp.obj: src/uml/CMakeFiles/Uml.dir/includes_CXX.rsp
src/uml/CMakeFiles/Uml.dir/namedElement.cpp.obj: ../src/uml/namedElement.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/uml/CMakeFiles/Uml.dir/namedElement.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Uml.dir\namedElement.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\namedElement.cpp

src/uml/CMakeFiles/Uml.dir/namedElement.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Uml.dir/namedElement.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\namedElement.cpp > CMakeFiles\Uml.dir\namedElement.cpp.i

src/uml/CMakeFiles/Uml.dir/namedElement.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Uml.dir/namedElement.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\namedElement.cpp -o CMakeFiles\Uml.dir\namedElement.cpp.s

src/uml/CMakeFiles/Uml.dir/class.cpp.obj: src/uml/CMakeFiles/Uml.dir/flags.make
src/uml/CMakeFiles/Uml.dir/class.cpp.obj: src/uml/CMakeFiles/Uml.dir/includes_CXX.rsp
src/uml/CMakeFiles/Uml.dir/class.cpp.obj: ../src/uml/class.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/uml/CMakeFiles/Uml.dir/class.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Uml.dir\class.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\class.cpp

src/uml/CMakeFiles/Uml.dir/class.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Uml.dir/class.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\class.cpp > CMakeFiles\Uml.dir\class.cpp.i

src/uml/CMakeFiles/Uml.dir/class.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Uml.dir/class.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\class.cpp -o CMakeFiles\Uml.dir\class.cpp.s

src/uml/CMakeFiles/Uml.dir/classifier.cpp.obj: src/uml/CMakeFiles/Uml.dir/flags.make
src/uml/CMakeFiles/Uml.dir/classifier.cpp.obj: src/uml/CMakeFiles/Uml.dir/includes_CXX.rsp
src/uml/CMakeFiles/Uml.dir/classifier.cpp.obj: ../src/uml/classifier.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/uml/CMakeFiles/Uml.dir/classifier.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Uml.dir\classifier.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\classifier.cpp

src/uml/CMakeFiles/Uml.dir/classifier.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Uml.dir/classifier.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\classifier.cpp > CMakeFiles\Uml.dir\classifier.cpp.i

src/uml/CMakeFiles/Uml.dir/classifier.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Uml.dir/classifier.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\classifier.cpp -o CMakeFiles\Uml.dir\classifier.cpp.s

src/uml/CMakeFiles/Uml.dir/property.cpp.obj: src/uml/CMakeFiles/Uml.dir/flags.make
src/uml/CMakeFiles/Uml.dir/property.cpp.obj: src/uml/CMakeFiles/Uml.dir/includes_CXX.rsp
src/uml/CMakeFiles/Uml.dir/property.cpp.obj: ../src/uml/property.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/uml/CMakeFiles/Uml.dir/property.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Uml.dir\property.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\property.cpp

src/uml/CMakeFiles/Uml.dir/property.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Uml.dir/property.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\property.cpp > CMakeFiles\Uml.dir\property.cpp.i

src/uml/CMakeFiles/Uml.dir/property.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Uml.dir/property.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\property.cpp -o CMakeFiles\Uml.dir\property.cpp.s

src/uml/CMakeFiles/Uml.dir/type.cpp.obj: src/uml/CMakeFiles/Uml.dir/flags.make
src/uml/CMakeFiles/Uml.dir/type.cpp.obj: src/uml/CMakeFiles/Uml.dir/includes_CXX.rsp
src/uml/CMakeFiles/Uml.dir/type.cpp.obj: ../src/uml/type.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/uml/CMakeFiles/Uml.dir/type.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Uml.dir\type.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\type.cpp

src/uml/CMakeFiles/Uml.dir/type.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Uml.dir/type.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\type.cpp > CMakeFiles\Uml.dir\type.cpp.i

src/uml/CMakeFiles/Uml.dir/type.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Uml.dir/type.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\type.cpp -o CMakeFiles\Uml.dir\type.cpp.s

src/uml/CMakeFiles/Uml.dir/typedElement.cpp.obj: src/uml/CMakeFiles/Uml.dir/flags.make
src/uml/CMakeFiles/Uml.dir/typedElement.cpp.obj: src/uml/CMakeFiles/Uml.dir/includes_CXX.rsp
src/uml/CMakeFiles/Uml.dir/typedElement.cpp.obj: ../src/uml/typedElement.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/uml/CMakeFiles/Uml.dir/typedElement.cpp.obj"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Uml.dir\typedElement.cpp.obj -c C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\typedElement.cpp

src/uml/CMakeFiles/Uml.dir/typedElement.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Uml.dir/typedElement.cpp.i"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\typedElement.cpp > CMakeFiles\Uml.dir\typedElement.cpp.i

src/uml/CMakeFiles/Uml.dir/typedElement.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Uml.dir/typedElement.cpp.s"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonme\ALL_PROJECTS\yuml\src\uml\typedElement.cpp -o CMakeFiles\Uml.dir\typedElement.cpp.s

# Object files for target Uml
Uml_OBJECTS = \
"CMakeFiles/Uml.dir/namespace.cpp.obj" \
"CMakeFiles/Uml.dir/model.cpp.obj" \
"CMakeFiles/Uml.dir/element.cpp.obj" \
"CMakeFiles/Uml.dir/namedElement.cpp.obj" \
"CMakeFiles/Uml.dir/class.cpp.obj" \
"CMakeFiles/Uml.dir/classifier.cpp.obj" \
"CMakeFiles/Uml.dir/property.cpp.obj" \
"CMakeFiles/Uml.dir/type.cpp.obj" \
"CMakeFiles/Uml.dir/typedElement.cpp.obj"

# External object files for target Uml
Uml_EXTERNAL_OBJECTS =

src/uml/libUml.a: src/uml/CMakeFiles/Uml.dir/namespace.cpp.obj
src/uml/libUml.a: src/uml/CMakeFiles/Uml.dir/model.cpp.obj
src/uml/libUml.a: src/uml/CMakeFiles/Uml.dir/element.cpp.obj
src/uml/libUml.a: src/uml/CMakeFiles/Uml.dir/namedElement.cpp.obj
src/uml/libUml.a: src/uml/CMakeFiles/Uml.dir/class.cpp.obj
src/uml/libUml.a: src/uml/CMakeFiles/Uml.dir/classifier.cpp.obj
src/uml/libUml.a: src/uml/CMakeFiles/Uml.dir/property.cpp.obj
src/uml/libUml.a: src/uml/CMakeFiles/Uml.dir/type.cpp.obj
src/uml/libUml.a: src/uml/CMakeFiles/Uml.dir/typedElement.cpp.obj
src/uml/libUml.a: src/uml/CMakeFiles/Uml.dir/build.make
src/uml/libUml.a: src/uml/CMakeFiles/Uml.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\jonme\ALL_PROJECTS\yuml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX static library libUml.a"
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && $(CMAKE_COMMAND) -P CMakeFiles\Uml.dir\cmake_clean_target.cmake
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Uml.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/uml/CMakeFiles/Uml.dir/build: src/uml/libUml.a

.PHONY : src/uml/CMakeFiles/Uml.dir/build

src/uml/CMakeFiles/Uml.dir/clean:
	cd /d C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml && $(CMAKE_COMMAND) -P CMakeFiles\Uml.dir\cmake_clean.cmake
.PHONY : src/uml/CMakeFiles/Uml.dir/clean

src/uml/CMakeFiles/Uml.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\jonme\ALL_PROJECTS\yuml C:\Users\jonme\ALL_PROJECTS\yuml\src\uml C:\Users\jonme\ALL_PROJECTS\yuml\build C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml C:\Users\jonme\ALL_PROJECTS\yuml\build\src\uml\CMakeFiles\Uml.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src/uml/CMakeFiles/Uml.dir/depend

