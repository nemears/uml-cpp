#include "headers/namespaceParser.h"

bool NamespaceParser::parseFeatures(YAML::Node node, UML::Element* el) {
    return NamedElementParser::parseFeatures(node, el);
}