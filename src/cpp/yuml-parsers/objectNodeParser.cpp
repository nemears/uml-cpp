#include "yuml-parsers/objectNodeParser.h"

bool ObjectNodeParser::parseFeatures(YAML::Node node, Element* el) {

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

    bool ret = emitType(emitter, el);

    if (!emitActivityNode(emitter, el)) {
        ret = false;
    }

    if (el->getElementType() == ElementType::OBJECT_NODE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}