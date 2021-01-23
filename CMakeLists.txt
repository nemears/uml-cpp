cmake_minimum_required(VERSION 3.0.0)
project(yuml VERSION 0.1.0)
include(CTest)
enable_testing()

set(CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

set(Boost_DEBUG 1)

FIND_PACKAGE( Boost 1.71.0 REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_subdirectory(libs/googletest)

add_subdirectory(libs/yaml-cpp)

add_subdirectory(src)

add_executable(yuml src/cpp/application.cpp)

target_link_libraries(yuml PUBLIC Parsers Uml Logger gtest yaml-cpp)
#                           PRIVATE yaml-cpp)
TARGET_LINK_LIBRARIES(yuml LINK_PUBLIC ${Boost_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
