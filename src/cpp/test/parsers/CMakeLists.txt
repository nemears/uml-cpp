# add_executable(parsersTest  opaqueBehaviorParserTest.cpp parameterParserTest.cpp
#                            operationParserTest.cpp structuredClassifierParserTest.cpp  primitiveTypeParserTest.cpp
#                            instanceSpecificationParserTest.cpp enumerationParserTest.cpp expressionParserTest.cpp modelParserTest.cpp
#                            literalsParserTest.cpp templateableElementParserTest.cpp associationParserTest.cpp profileParserTest.cpp
#                            commentParserTest.cpp dependencyParserTest.cpp deploymentParserTest.cpp behavioredClassifierParserTest.cpp
#                            generalizationSetParserTest.cpp)
add_executable(parsersTest packageParserTest.cpp classParserTest.cpp propertyParserTest.cpp dataTypeParserTest.cpp)
target_link_libraries(parsersTest PUBLIC gtest gmock gtest_main Uml Parsers yaml-cpp)
add_test(parsersTest parsersTest)