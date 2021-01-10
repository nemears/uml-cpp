#include "headers/classParser.h"

UML::Element* ClassParser::createElement() {
    return new UML::Class;
}

bool ClassParser::parseFeatures(YAML::Node node, UML::Element* el) {
    return ClassifierParser::parseFeatures(node, el);
}