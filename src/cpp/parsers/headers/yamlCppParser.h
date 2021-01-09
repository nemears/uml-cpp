#ifndef YAMLCPPPARSER
#define YAMLCPPPARSER
#include "inputParser.h"
#include "yaml-cpp/yaml.h"

class YamlCppParser : public InputParser {
    private:
        YAML::Node modelNode;
    public:
        YamlCppParser(string f) : InputParser(f) {
            //ifstream fin(f);
            YAML::Node modelNode = YAML::LoadFile(f);
        }
        bool readNextLine();
};

#endif