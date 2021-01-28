#include "yuml-parsers/parameterParser.h"

Element* ParameterParser::createElement() {
    return new Parameter;
}

bool ParameterParser::parseFeatures(YAML::Node node, Element* el) {
    return TypedElementParser::parseFeatures(node, el);
}