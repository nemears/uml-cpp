add_library(Parsers parser.cpp packageParser.cpp)
target_link_libraries(Parsers PUBLIC yaml-cpp)