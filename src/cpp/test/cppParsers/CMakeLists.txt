add_executable(cppParserTest cppParserTest.cpp)
target_link_libraries(cppParserTest PUBLIC gtest gmock gtest_main Uml Parsers yaml-cpp cppParsers clang)
add_test(cppParserTest cppParserTest)