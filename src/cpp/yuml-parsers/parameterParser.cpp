#include "yuml-parsers/parameterParser.h"

Element* ParameterParser::createElement() {
    return new Parameter;
}

bool ParameterParser::parseFeatures(YAML::Node node, Element* el) {
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