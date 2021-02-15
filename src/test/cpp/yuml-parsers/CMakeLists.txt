add_executable(parserTest elementParserTest.cpp namedElementParserTest.cpp typedElementParserTest.cpp classifierParserTest.cpp 
                          propertyParserTest.cpp instanceSpecificationParserTest.cpp classParserTest.cpp operationParserTest.cpp
                          parameterParserTest.cpp slotParserTest.cpp multiplicityElementParserTest.cpp controlFlowParserTest.cpp)
target_link_libraries(parserTest PUBLIC gtest gmock gtest_main Parsers Uml)
add_test(parserTest parserTest)