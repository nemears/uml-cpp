#include "yuml-parsers/joinNodeParser.h"

Element* JoinNodeParser::createElement() {
    theEl =  new JoinNode;
    return theEl;
}

bool JoinNodeParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::JOIN_NODE) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "joinNode";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = NamedElementParser::emit(emitter, el);
    
    if (!emitActivityNode(emitter, el)) {
        ret = false;
    }

    if (el->getElementType() == ElementType::JOIN_NODE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

JoinNodeParser JoinNodeParser::createNewParser() {
    return JoinNodeParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}