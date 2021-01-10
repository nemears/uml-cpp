#include "headers/classParser.h"

Element* ClassParser::createElement() {
    return new Class;
}

bool ClassParser::parseFeatures(YAML::Node node, Element* el) {
    return ClassifierParser::parseFeatures(node, el);
}