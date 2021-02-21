add_library(Parsers templateParser.cpp namespaceParser.cpp modelParser.cpp elementParser.cpp namedElementParser.cpp classParser.cpp 
                    classifierParser.cpp propertyParser.cpp typedElementParser.cpp instanceSpecificationParser.cpp slotParser.cpp
                    operationParser.cpp parameterParser.cpp opaqueBehaviorParser.cpp activityParser.cpp activityNodeParser.cpp
                    multiplicityElementParser.cpp objectNodeParser.cpp actionParser.cpp pinParser.cpp inputPinParser.cpp outputPinParser.cpp
                    activityEdgeParser.cpp controlFlowParser.cpp objectFlowParser.cpp initialNodeParser.cpp finalNodeParser.cpp
                    decisionNodeParser.cpp joinNodeParser.cpp forkNodeParser.cpp)
target_link_libraries(Parsers PUBLIC yaml-cpp)