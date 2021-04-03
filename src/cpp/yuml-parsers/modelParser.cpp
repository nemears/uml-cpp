#include "yuml-parsers/modelParser.h"

UML::Element* ModelParser::createElement() {
    NamespaceParser::theEl = new UML::Model;
    return NamespaceParser::theEl;
}

bool ModelParser::parseFeatures(YAML::Node node, UML::Element* el) {
    return PackageParser::parseFeatures(node, el);
}

bool ModelParser::emit(YAML::Emitter& emitter, Element* el) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "model";
    emitter << YAML::BeginMap;
    bool ret = PackageParser::emit(emitter, el);
    emitter << YAML::EndMap;
    return ret;
}

ModelParser ModelParser::createNewParser() {
    return ModelParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}

void ModelParser::deleteParser(TemplateParser* parser) {
    delete parser->elements;
    delete parser->postProcessFlag;
}