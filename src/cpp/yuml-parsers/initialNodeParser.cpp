#include "yuml-parsers/initialNodeParser.h"

Element* InitialNodeParser::createElement() {
    theEl = new InitialNode;
    return theEl;
}

bool InitialNodeParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::INITIAL_NODE) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "initialNode";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = NamedElementParser::emit(emitter, el);
    
    if (!emitActivityNode(emitter, el)) {
        ret = false;
    }

    if (el->getElementType() == ElementType::INITIAL_NODE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

InitialNodeParser InitialNodeParser::createNewParser() {
    return InitialNodeParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}