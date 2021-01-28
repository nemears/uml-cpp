#include "yuml-parsers/opaqueBehaviorParser.h"

Element* OpaqueBehaviorParser::createElement() {
    return new OpaqueBehavior;
}

bool OpaqueBehaviorParser::parseFeatures(YAML::Node node, Element* el) {

    // just 1 for now TODO
    if (node["body"]) {
        ((OpaqueBehavior*) el)->bodies.push_back(node["body"].as<string>());
    }

    return ClassParser::parseFeatures(node, el);
}