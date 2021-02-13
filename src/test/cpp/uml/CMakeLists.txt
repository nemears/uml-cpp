add_executable(umltest elementTest.cpp namedElementTest.cpp namespaceTest.cpp classifierTest.cpp typedElementTest.cpp 
                       valueSpecificationTest.cpp propertyTest.cpp instanceSpecificationTest.cpp classTest.cpp 
                       parameterTest.cpp operationTest.cpp activityEdgeTest.cpp multiplicityElementTest.cpp)
target_link_libraries(umltest PUBLIC gtest gmock gtest_main Uml)
add_test(umltest umltest)