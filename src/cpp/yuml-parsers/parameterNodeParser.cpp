#include "yuml-parsers/parameterNodeParser.h"

bool ParameterNodeParser::parseFeatures(YAML::Node node, Element* el) {

    bool ret = NamedElementParser::parseFeatures(node, el);

    if (node["parameter"]) {
        if (node["parameter"].IsScalar()) {
            if (isValidUUID4(node["parameter"].as<string>())) {
                boost::uuids::uuid paramId = boost::lexical_cast<boost::uuids::uuid>(node["parameter"].as<string>());

                parseNowOrLater(paramId, el->uuid, &ParameterNodeParser::setParameterLater);
            }
        }
    }

    if (!parseObjectNodeFeatures(node, el)) {
        ret = false;
    }

    return ret;
}

Element* ParameterNodeParser::createElement() {
    theEl = new ParameterNode;
    return theEl;
}

bool ParameterNodeParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::PARAMETER_NODE) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "parameterNode";
        emitter << YAML::Value << YAML::BeginMap;
    }

    if (dynamic_cast<ParameterNode*>(el)->getParameter() != NULL) {
        emitter << YAML::Key << "parameter";
        emitter << YAML::Value << boost::lexical_cast<string>(dynamic_cast<ParameterNode*>(el)->getParameter()->uuid);
    }

    bool ret = ObjectNodeParser::emit(emitter, el);

    if (el->getElementType() == ElementType::PARAMETER_NODE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }
    
    return ret;
}