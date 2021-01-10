#include "headers/propertyParser.h"

UML::Element* PropertyParser::createElement(){
    return new UML::Property;
}

bool PropertyParser::parseFeatures(YAML::Node node, UML::Element* el) {
    if (node["defaultValue"]) {
        // TODO
    }

    return TypedElementParser::parseFeatures(node, el);
}