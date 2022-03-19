add_executable(parsersTest packageParserTest.cpp classParserTest.cpp propertyParserTest.cpp dataTypeParserTest.cpp 
               primitiveTypeParserTest.cpp opaqueBehaviorParserTest.cpp parameterParserTest.cpp operationParserTest.cpp 
               structuredClassifierParserTest.cpp instanceSpecificationParserTest.cpp enumerationParserTest.cpp 
               expressionParserTest.cpp modelParserTest.cpp literalsParserTest.cpp templateableElementParserTest.cpp
               associationParserTest.cpp profileParserTest.cpp commentParserTest.cpp dependencyParserTest.cpp
               deploymentParserTest.cpp behavioredClassifierParserTest.cpp generalizationSetParserTest.cpp
               interfaceParserTest.cpp signalParserTest.cpp activityParserTest.cpp)
target_link_libraries(parsersTest PUBLIC gtest gmock gtest_main uml yaml-cpp)
add_test(parsersTest parsersTest)