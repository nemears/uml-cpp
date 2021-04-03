#include "yuml-parsers/packageableElementParser.h"

bool PackageableElementParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = NamedElementParser::parseFeatures(node, el);

    // TODO

    return ret;
}

bool PackageableElementParser::emit(YAML::Emitter& emitter, Element* el) {
    bool ret = NamedElementParser::emit(emitter, el);

    return ret;
}