#include "yuml-parsers/parameterParser.h"

Element* ParameterParser::createElement() {
    return new Parameter;
}

bool ParameterParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["direction"]) {
        string directionString = node["direction"].as<string>();

        if (directionString.compare("IN") == 0) {
            ((Parameter*)el)->setDirection(ParameterDirectionKind::IN);
        } else if (directionString.compare("OUT") == 0) {
            ((Parameter*)el)->setDirection(ParameterDirectionKind::OUT);
        } else if (directionString.compare("INOUT") == 0) {
            ((Parameter*)el)->setDirection(ParameterDirectionKind::INOUT);
        } else if (directionString.compare("RETURN")) {
            ((Parameter*)el)->setDirection(ParameterDirectionKind::RETURN);
        } else {
            // TODO ERROR
        }
    }

    return TypedElementParser::parseFeatures(node, el);
}

bool ParameterParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::PARAMETER) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "parameter";
        emitter << YAML::BeginMap;
    }

    bool ret = TypedElementParser::emit(emitter, el);

    if (el->getElementType() == ElementType::PARAMETER) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}