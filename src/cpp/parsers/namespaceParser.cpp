#include "headers/namespaceParser.h"

bool NamespaceParser::parseFeatures(YAML::Node node, Element* el) {
    return NamedElementParser::parseFeatures(node, el);
}