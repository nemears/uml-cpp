#include "yuml-parsers/parameterParser.h"

Element* ParameterParser::createElement() {
    return new Parameter;
}

bool ParameterParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["lower"] && node["upper"]) {
        if (!parseMultiplicityFeatures(node, el)) {
            return false;
        }
    }

    if (node["direction"]) {
        string directionString = node["direction"].as<string>();

        if (directionString.compare("IN") == 0) {
            dynamic_cast<Parameter*>(el)->setDirection(ParameterDirectionKind::IN);
        } else if (directionString.compare("OUT") == 0) {
            dynamic_cast<Parameter*>(el)->setDirection(ParameterDirectionKind::OUT);
        } else if (directionString.compare("INOUT") == 0) {
            dynamic_cast<Parameter*>(el)->setDirection(ParameterDirectionKind::INOUT);
        } else if (directionString.compare("RETURN")) {
            dynamic_cast<Parameter*>(el)->setDirection(ParameterDirectionKind::RETURN);
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

    if (!emitMultiplicity(emitter, el)) {
        ret = false;
    }

    if (dynamic_cast<Parameter*>(el)->getDirection() != ParameterDirectionKind::NONE) {
        emitter << YAML::Key << "direction";
        
        switch (dynamic_cast<Parameter*>(el)->getDirection()) {
            case ParameterDirectionKind::IN : {
                emitter << YAML::Value << "IN";
                break;
            }
            case ParameterDirectionKind::OUT : {
                emitter << YAML::Value << "OUT";
                break;
            }
            case ParameterDirectionKind::INOUT : {
                emitter << YAML::Value << "INOUT";
                break;
            }
            case ParameterDirectionKind::RETURN : {
                emitter << YAML::Value << "RETURN";
                break;
            }
        }
    }

    if (el->getElementType() == ElementType::PARAMETER) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}