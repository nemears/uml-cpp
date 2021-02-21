#include "yuml-parsers/forkNodeParser.h"

Element* ForkNodeParser::createElement() {
    theEl =  new ForkNode;
    return theEl;
}

bool ForkNodeParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::FORK_NODE) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "forkNode";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = NamedElementParser::emit(emitter, el);
    
    if (!emitActivityNode(emitter, el)) {
        ret = false;
    }

    if (el->getElementType() == ElementType::FORK_NODE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

ForkNodeParser ForkNodeParser::createNewParser() {
    return ForkNodeParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}