add_executable(parserTest elementParserTest.cpp)
target_link_libraries(parserTest PUBLIC gtest gmock gtest_main Parsers Uml)
add_test(parserTest parserTest)