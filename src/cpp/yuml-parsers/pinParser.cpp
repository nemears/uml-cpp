#include "yuml-parsers/pinParser.h"

bool PinParser::parseFeatures(YAML::Node node, Element* el) {

    bool ret = NamedElementParser::parseFeatures(node, el);
    
    if(!parseObjectNodeFeatures(node, el)) {
        ret = false;
    }

    if (!parseMultiplicityFeatures(node, el)) {
        ret = false;
    }

    return ret;
}

bool PinParser::emit(YAML::Emitter& emitter, Element* el) {
    
    bool ret = emitObjectNode(emitter, el);

    if(!emitMultiplicity(emitter, el)) {
        ret = false;
    }

    return ret;
}