cmake_minimum_required(VERSION 3.0.0)
project(uml-cpp VERSION 0.1.0)
include(CTest)
enable_testing()

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

IF(UNIX)
    SET(GCC_COMPILER_FLAGS "-fPIC")
    add_definitions(${GCC_COMPILER_FLAGS})
ENDIF()

# llvm clang dependency
find_package(LLVM 10 REQUIRED)
# find_package(Clang REQUIRED)
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIR})
add_definitions(${LLVM_DEFINITIONS})
add_definitions(${CLANG_DEFINITIONS})

# Headers
include_directories(include)

# Google Test dependency
IF(WIN32)
    set(gtest_force_shared_crt ON)
ENDIF()
add_subdirectory(libs/googletest)

# yaml-cpp dependency
add_subdirectory(libs/yaml-cpp)
set(YAML_CPP_BUILD_TESTS OFF)

# pybind11
add_subdirectory(libs/pybind11)

# src
add_subdirectory(src)

# wrapper application
add_executable(uml-cpp src/cpp/application.cpp)

# Link them all
target_link_libraries(uml-cpp PUBLIC Parsers Uml cppParsers gtest yaml-cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
