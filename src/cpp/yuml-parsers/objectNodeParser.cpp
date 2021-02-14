#include "yuml-parsers/objectNodeParser.h"

bool ObjectNodeParser::parseFeatures(YAML::Node node, Element* el) {

    bool ret = parseTypeFeatures(node, el);

    if (!parseActivityNodeFeatures(node, el)) {
        ret = false;
    }

    return ret;
}

bool ObjectNodeParser::emit(YAML::Emitter& emitter, Element* el) {

    bool ret = emitType(emitter, el);

    if (!emitActivityNode(emitter, el)) {
        ret = false;
    }

    return ret;
}