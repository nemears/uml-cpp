add_executable(umltest idTest.cpp elementTest.cpp namedElementTest.cpp namespaceTest.cpp packageTest.cpp classifierTest.cpp setTest.cpp
                       dependencyTest.cpp generalizationTest.cpp dataTypeTest.cpp propertyTest.cpp valueSpecificationTest.cpp
                       typedElementTest.cpp instanceSpecificationTest.cpp slotTest.cpp classTest.cpp multiplicityElementTest.cpp
                       directedRelationshipTest.cpp operationTest.cpp parameterTest.cpp behaviorTest.cpp associationTest.cpp
                       enumerationTest.cpp commentTest.cpp templateableElementTest.cpp profileApplicationTest.cpp extensionTest.cpp
                       stereotypeTest.cpp behavioredClassifierTest.cpp deploymentTest.cpp manifestationTest.cpp
                       generalizationSetTest.cpp connectorTest.cpp interfaceTest.cpp portTest.cpp signalTest.cpp UmlManagerTest.cpp
                       activityEdgeTest.cpp activityTest.cpp activityNodeTest.cpp objectNodeTest.cpp inputPinTest.cpp outputPinTest.cpp 
)
if (UML_CLIENT_SERVER)
    target_sources(umltest PRIVATE restfulTest.cpp)
endif()
target_link_libraries(umltest PUBLIC gtest gmock gtest_main uml)
add_test(umltest umltest)