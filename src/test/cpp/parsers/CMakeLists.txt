add_executable(parserTest elementParserTest.cpp namedElementParserTest.cpp)
target_link_libraries(parserTest PUBLIC gtest gmock gtest_main Parsers Uml)
add_test(parserTest parserTest)