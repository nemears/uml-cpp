#include "headers/operationParser.h"

Element* OperationParser::createElement() {
    return new Operation;
}

bool OperationParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["parameters"]) {
        if (node["parameters"].IsSequence()) {
            for (std::size_t i=0; i<node["parameters"].size(); i++) {
                ParameterParser parameterParser(elements);
                Element* parsedEl = parameterParser.parseElement(node["parameters"][i]["parameter"]);
                ((Operation*) el)->parameters.push_back((Parameter*) parsedEl);
            }
        }
    }

    if (node["methods"]) {
        if (node["methods"].IsSequence()) {
            for (std::size_t i=0; i<node["methods"].size(); i++) {
                // TODO opaqueBehaviorParser
            }
        }
    }

    return NamedElementParser::parseFeatures(node, el);
}