#include "yuml-parsers/mergeNodeParser.h"

Element* MergeNodeParser::createElement() {
    theEl =  new MergeNode;
    return theEl;
}

bool MergeNodeParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::MERGE_NODE) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "mergeNode";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = NamedElementParser::emit(emitter, el);
    
    if (!emitActivityNode(emitter, el)) {
        ret = false;
    }

    if (el->getElementType() == ElementType::MERGE_NODE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

MergeNodeParser MergeNodeParser::createNewParser() {
    return MergeNodeParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}