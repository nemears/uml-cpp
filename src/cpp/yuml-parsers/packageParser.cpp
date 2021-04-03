#include "yuml-parsers/packageParser.h"
#include "uml/package.h"

bool PackageParser::parseFeatures(YAML::Node node, Element* el) {
    return true;
}

Element* PackageParser::createElement() {
    NamespaceParser::theEl = new Package;
    return NamespaceParser::theEl;
}

bool PackageParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::PACKAGE) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "package";
        emitter << YAML::Value << YAML::BeginMap;
    }

    if (el->getElementType() == ElementType::PACKAGE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return true;
}