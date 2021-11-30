add_executable(umltest setTest.cpp elementTest.cpp namedElementTest.cpp namespaceTest.cpp packageTest.cpp dependencyTest.cpp
                       classifierTest.cpp generalizationTest.cpp dataTypeTest.cpp propertyTest.cpp valueSpecificationTest.cpp
                       typedElementTest.cpp instanceSpecificationTest.cpp slotTest.cpp classTest.cpp multiplicityElementTest.cpp
                       directedRelationshipTest.cpp operationTest.cpp)
                    #    parameterTest.cpp activityEdgeTest.cpp 
                    #    objectNodeTest.cpp activityNodeTest.cpp inputPinTest.cpp outputPinTest.cpp  
                    #     behaviorTest.cpp activityTest.cpp 
                    #    associationTest.cpp enumerationTest.cpp commentTest.cpp idTest.cpp UmlManagerTest.cpp 
                    #    templateableElementTest.cpp profileApplicationTest.cpp extensionTest.cpp stereotypeTest.cpp
                    #     deploymentTest.cpp behavioredClassifierTest.cpp manifestationTest.cpp generalizationSetTest.cpp)
if (UML_CLIENT_SERVER)
    target_sources(umltest PRIVATE restfulTest.cpp)
endif()
target_link_libraries(umltest PUBLIC gtest gmock gtest_main Uml)
add_test(umltest umltest)