cmake_minimum_required(VERSION 3.1.0)
project(uml-cpp VERSION 0.1.0)
include(CTest)
enable_testing()

option(UML_CPP_BUILD_CLANG "allows for building the src/cpp/uml/cpp target if llvm is findable" ON)
option(UML_CLIENT_SERVER "allows for building of src/cpp/uml/managers for restful server client classes" ON)

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

IF(UNIX)
    SET(GCC_COMPILER_FLAGS "-fPIC")
    add_definitions(${GCC_COMPILER_FLAGS})
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
ENDIF()

if (UML_CPP_BUILD_CLANG) 
    # llvm clang dependency
    find_package(LLVM 10 REQUIRED)
    # find_package(Clang REQUIRED)
    include_directories(${LLVM_INCLUDE_DIRS})
    link_directories(${LLVM_LIBRARY_DIR})
    add_definitions(${LLVM_DEFINITIONS})
    add_definitions(${CLANG_DEFINITIONS})
endif()

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

# src
add_subdirectory(src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
