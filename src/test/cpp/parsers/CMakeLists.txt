add_executable(parserTest elementParserTest.cpp)
target_link_libraries(parserTest PUBLIC gtest gmock gtest_main Uml)
add_test(parserTest parserTest)