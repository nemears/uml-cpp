cmake_minimum_required(VERSION 3.12.0)
project(uml-cpp VERSION 0.1.0)
include(CTest)
enable_testing()

option(UML_CLIENT_SERVER "allows for building of src/cpp/uml/managers for restful server client classes" ON)
option(UML_BUILD_TESTS "build tests for uml library" ON)

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Threads REQUIRED)

IF(UNIX)
    option(UML_DEBUG "builds without optimizations" OFF)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    add_compile_options(-Wall -Wextra -pedantic)
    IF(NOT UML_DEBUG) 
        add_compile_options(-O3)
    ENDIF()
ENDIF()

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
add_subdirectory(src/cpp/uml)
if(UML_CLIENT_SERVER)
    list(APPEND umlSources managers/umlClient.cpp managers/umlServer.cpp)
else() 
    add_definitions( -DNO_UML_CLIENT_SERVER)
endif()
list(TRANSFORM umlSources PREPEND "src/cpp/uml/")
add_library(uml STATIC ${umlSources})
target_include_directories(uml PUBLIC include)
target_link_libraries(uml PUBLIC yaml-cpp)
if (UML_CLIENT_SERVER)
    add_executable(uml-server src/cpp/uml/managers/main.cpp)
    target_link_libraries(uml-server PUBLIC uml)
    target_link_libraries(uml-server PRIVATE Threads::Threads) #  -fsanitize=address
    if(WIN32)
        target_link_libraries(uml-server PRIVATE wsock32 ws2_32)
        target_link_libraries(uml PRIVATE wsock32 ws2_32)
    endif()
endif()
if (UML_BUILD_TESTS)
    add_subdirectory(src/cpp/test)
    list(TRANSFORM umltestSources PREPEND "src/cpp/test/")
    if (UML_CLIENT_SERVER)
        list(APPEND umltestSources src/cpp/test/restfulTest.cpp)
    endif()
    add_executable(umltest ${umltestSources})
    target_link_libraries(umltest PUBLIC gtest gmock gtest_main uml)
    add_test(umltest umltest)
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
