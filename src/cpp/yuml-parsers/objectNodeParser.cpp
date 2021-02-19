#include "yuml-parsers/objectNodeParser.h"

Element* ObjectNodeParser::createElement() {
    theEl = new ObjectNode;
    return theEl;
}

bool ObjectNodeParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = NamedElementParser::parseFeatures(node, el);

    if(!parseObjectNodeFeatures(node, el)) {
        ret = false;
    }

    return ret;
}

bool ObjectNodeParser::parseObjectNodeFeatures(YAML::Node node, Element* el) {
    
    bool ret = parseTypeFeatures(node, el);

    if (!parseActivityNodeFeatures(node, el)) {
        ret = false;
    }

    return ret;
}

bool ObjectNodeParser::emit(YAML::Emitter& emitter, Element* el) {

    if (el->getElementType() == ElementType::OBJECT_NODE) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "objectNode";
        emitter << YAML::BeginMap;
    }

    bool ret = emitObjectNode(emitter, el);

    if (el->getElementType() == ElementType::OBJECT_NODE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

bool ObjectNodeParser::emitObjectNode(YAML::Emitter& emitter, Element* el) {
    bool ret = emitType(emitter, el);

    if (!emitActivityNode(emitter, el)) {
        ret = false;
    }

    return ret;
}

ObjectNodeParser ObjectNodeParser::createNewParser() {
    return ObjectNodeParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}