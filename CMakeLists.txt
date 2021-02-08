cmake_minimum_required(VERSION 3.0.0)
project(yuml VERSION 0.1.0)
include(CTest)
enable_testing()

set(CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

SET(GCC_COMPILER_FLAGS "-fPIC")
add_definitions(${GCC_COMPILER_FLAGS})

# Boost dependencies
set(Boost_DEBUG 1)
FIND_PACKAGE( Boost 1.71.0 REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# llvm clang dependency
find_package(Clang REQUIRED)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
add_definitions(${CLANG_DEFINITIONS})

# Headers
include_directories(include)

# Google Test dependency
add_subdirectory(libs/googletest)

# yaml-cpp dependency
add_subdirectory(libs/yaml-cpp)

# pybind11
add_subdirectory(libs/pybind11)

# src
add_subdirectory(src)

# wrapper application
add_executable(yuml src/cpp/application.cpp)

# Link them all
target_link_libraries(yuml PUBLIC Parsers Uml Logger gtest yaml-cpp)
TARGET_LINK_LIBRARIES(yuml LINK_PUBLIC ${Boost_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
