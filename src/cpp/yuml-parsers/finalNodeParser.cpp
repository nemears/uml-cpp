#include "yuml-parsers/finalNodeParser.h"

Element* FinalNodeParser::createElement() {
    theEl =  new FinalNode;
    return theEl;
}

bool FinalNodeParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::FINAL_NODE) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "finalNode";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = NamedElementParser::emit(emitter, el);
    
    if (!emitActivityNode(emitter, el)) {
        ret = false;
    }

    if (el->getElementType() == ElementType::FINAL_NODE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

FinalNodeParser FinalNodeParser::createNewParser() {
    return FinalNodeParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}