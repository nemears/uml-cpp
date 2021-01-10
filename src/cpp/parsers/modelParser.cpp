#include "headers/modelParser.h"

UML::Element* ModelParser::createElement() {
    return new UML::Model;
}

bool ModelParser::parseFeatures(YAML::Node node, UML::Element* el) {
    return NamespaceParser::parseFeatures(node, el);
}