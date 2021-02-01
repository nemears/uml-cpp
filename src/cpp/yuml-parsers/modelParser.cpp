#include "yuml-parsers/modelParser.h"

UML::Element* ModelParser::createElement() {
    theEl = new UML::Model;
    return theEl;
}

bool ModelParser::parseFeatures(YAML::Node node, UML::Element* el) {
    return NamespaceParser::parseFeatures(node, el);
}

bool ModelParser::emit(YAML::Emitter& emitter, Element* el) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "model";
    emitter << YAML::BeginMap;
    bool ret = NamespaceParser::emit(emitter, el);
    emitter << YAML::EndMap;
    return ret;
}