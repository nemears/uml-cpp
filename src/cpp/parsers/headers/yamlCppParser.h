#ifndef YAMLCPPPARSER
#define YAMLCPPPARSER
#include "inputParser.h"
#include "yaml-cpp/yaml.h"

class YamlCppParser : public InputParser {
    public:
        YamlCppParser(string f) : InputParser(f) {
            ifstream fin(f);
            YAML::Parser parser(fin);
        }
};

#endif