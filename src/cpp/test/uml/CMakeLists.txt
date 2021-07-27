add_executable(umltest elementTest.cpp namedElementTest.cpp sequenceTest.cpp namespaceTest.cpp classifierTest.cpp typedElementTest.cpp 
                       valueSpecificationTest.cpp propertyTest.cpp instanceSpecificationTest.cpp classTest.cpp 
                       parameterTest.cpp operationTest.cpp activityEdgeTest.cpp multiplicityElementTest.cpp
                       objectNodeTest.cpp activityNodeTest.cpp inputPinTest.cpp outputPinTest.cpp directedRelationshipTest.cpp 
                       slotTest.cpp behaviorTest.cpp activityTest.cpp generalizationTest.cpp packageTest.cpp dataTypeTest.cpp
                       associationTest.cpp enumerationTest.cpp commentTest.cpp idTest.cpp UmlManagerTest.cpp 
                       templateableElementTest.cpp profileApplicationTest.cpp extensionTest.cpp stereotypeTest.cpp
                       dependencyTest.cpp)
target_link_libraries(umltest PUBLIC gtest gmock gtest_main Uml)
add_test(umltest umltest)