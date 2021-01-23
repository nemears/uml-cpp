#include "headers/opaqueBehaviorParser.h"

Element* OpaqueBehaviorParser::createElement() {
    return new OpaqueBehavior;
}

bool OpaqueBehaviorParser::parseFeatures(YAML::Node node, Element* el) {
    return ClassParser::parseFeatures(node, el);
}