add_library(Parsers parser.cpp parserMetaData.cpp emitterMetaData.cpp)
target_link_libraries(Parsers PUBLIC yaml-cpp Uml)