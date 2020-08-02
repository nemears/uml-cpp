cmake_minimum_required(VERSION 3.0.0)
project(yuml VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(src)

add_executable(yuml src/application.cpp)

target_link_libraries(yuml PUBLIC Parsers)
#target_link_libraries(yuml Public Uml)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_AUTOMOC ON)
include(CPack)
