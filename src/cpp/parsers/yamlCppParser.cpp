#include "headers/yamlCppParser.h"

bool YamlCppParser::readNextLine() {
    // if (modelNode["model"]) {
    //     cout << "model found";
    // }
    cout << modelNode["model"].Type();
    return true;
}