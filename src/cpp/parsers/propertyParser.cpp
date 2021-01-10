#include "headers/propertyParser.h"

Element* PropertyParser::createElement(){
    return new Property;
}

bool PropertyParser::parseFeatures(YAML::Node node, Element* el) {
    if (node["defaultValue"]) {
        // TODO
    }

    return TypedElementParser::parseFeatures(node, el);
}