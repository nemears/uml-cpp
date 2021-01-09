#include "headers/modelParser.h"

Element* ModelParser::createElement() {
    return new Model;
}

bool ModelParser::parseTag(InputParser* input, Element* el) {
    return NamespaceParser::parseTag(input, el);
}

bool ModelParser::parseFeatures(YAML::Node node, Element* el) {
    return NamespaceParser::parseFeatures(node, el);
}