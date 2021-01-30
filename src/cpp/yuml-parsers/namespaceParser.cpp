#include "yuml-parsers/namespaceParser.h"

bool NamespaceParser::parseFeatures(YAML::Node node, UML::Element* el) {
    return NamedElementParser::parseFeatures(node, el);
}

bool NamespaceParser::emit(YAML::Emitter& emitter, Element* el) {
    bool ret = NamedElementParser::emit(emitter, el);
    return ret;
}