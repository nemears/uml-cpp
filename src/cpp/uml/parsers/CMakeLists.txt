add_library(Parsers parser.cpp packageParser.cpp packageableElementParser.cpp)
target_link_libraries(Parsers PUBLIC yaml-cpp)