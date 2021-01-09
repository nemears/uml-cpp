#include "headers/namespaceParser.h"

bool NamespaceParser::parseTag(InputParser* input, Element* el) {
    return NamedElementParser::parseTag(input, el);
}

bool NamespaceParser::parseFeatures(YAML::Node node, Element* el) {
    return NamedElementParser::parseFeatures(node, el);
}