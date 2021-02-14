#include "yuml-parsers/objectNodeParser.h"

bool ObjectNodeParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = parseTypeFeatures(node, el);
    
    if (!parseActivityNodeFeatures(node, el)) {
        ret = false;
    }

    return ret;
}