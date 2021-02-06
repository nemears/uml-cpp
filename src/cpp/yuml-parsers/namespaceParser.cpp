#include "yuml-parsers/namespaceParser.h"

using namespace UML;

Element* NamespaceParser::createElement() {
    return new Namespace;
}

bool NamespaceParser::parseFeatures(YAML::Node node, Element* el) {
    return NamedElementParser::parseFeatures(node, el);
}

bool NamespaceParser::emit(YAML::Emitter& emitter, Element* el) {
    bool ret = NamedElementParser::emit(emitter, el);
    return ret;
}