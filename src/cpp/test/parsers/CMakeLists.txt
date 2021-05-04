add_executable(parsersTest packageParserTest.cpp classParserTest.cpp propertyParserTest.cpp opaqueBehaviorParserTest.cpp parameterParserTest.cpp
                           operationParserTest.cpp structuredClassifierParserTest.cpp dataTypeParserTest.cpp primitiveTypeParserTest.cpp)
target_link_libraries(parsersTest PUBLIC gtest gmock gtest_main Uml Parsers yaml-cpp)
add_test(parsersTest parsersTest)