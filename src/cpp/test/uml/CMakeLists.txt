add_executable(umltest idTest.cpp setTest.cpp elementTest.cpp namedElementTest.cpp namespaceTest.cpp packageTest.cpp dependencyTest.cpp
                       classifierTest.cpp generalizationTest.cpp dataTypeTest.cpp propertyTest.cpp valueSpecificationTest.cpp
                       typedElementTest.cpp instanceSpecificationTest.cpp slotTest.cpp classTest.cpp multiplicityElementTest.cpp
                       directedRelationshipTest.cpp operationTest.cpp parameterTest.cpp behaviorTest.cpp associationTest.cpp
                       enumerationTest.cpp commentTest.cpp templateableElementTest.cpp profileApplicationTest.cpp extensionTest.cpp
                       stereotypeTest.cpp behavioredClassifierTest.cpp deploymentTest.cpp manifestationTest.cpp
                       generalizationSetTest.cpp)
                    #     activityEdgeTest.cpp 
                    #    objectNodeTest.cpp activityNodeTest.cpp inputPinTest.cpp outputPinTest.cpp  
                    #      activityTest.cpp 
                    #    UmlManagerTest.cpp )
if (UML_CLIENT_SERVER)
    target_sources(umltest PRIVATE restfulTest.cpp)
endif()
target_link_libraries(umltest PUBLIC gtest gmock gtest_main Uml)
add_test(umltest umltest)