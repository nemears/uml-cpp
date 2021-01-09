#include "headers/propertyParser.h"

Element* PropertyParser::createElement(){
    return new Property;
}

bool PropertyParser::parseTag(InputParser* input, Element* el) {

    if (input->firstWord.compare("defaultValue")) {
        // TODO process default value of property
    }

    return TypedElementParser::parseTag(input, el);
}

bool PropertyParser::parseFeatures(YAML::Node node, Element* el) {
    if (node["defaultValue"]) {
        // TODO
    }

    return TypedElementParser::parseFeatures(node, el);
}