add_library(Parsers parser.cpp packageParser.cpp packageableElementParser.cpp namedElementParser.cpp namespaceParser.cpp elementParser.cpp
                    classParser.cpp classifierParser.cpp typedElementParser.cpp propertyParser.cpp parserMetaData.cpp behaviorParser.cpp
                    opaqueBehaviorParser.cpp parameterParser.cpp)
target_link_libraries(Parsers PUBLIC yaml-cpp Uml)