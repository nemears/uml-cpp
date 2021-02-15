#include "yuml-parsers/actionParser.h"

bool ActionParser::parseFeatures(YAML::Node node, Element* el) {

    return ActivityNodeParser::parseFeatures(node, el);
}

bool ActionParser::emit(YAML::Emitter& emitter, Element* el) {
    bool ret = ActivityNodeParser::emit(emitter, el);

    return ret;
}