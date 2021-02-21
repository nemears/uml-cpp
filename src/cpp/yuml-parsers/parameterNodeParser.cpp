#include "yuml-parsers/parameterNodeParser.h"

bool ParameterNodeParser::parseFeatures(YAML::Node node, Element* el) {

    bool ret = ObjectNodeParser::parseFeatures(node, el);

    if (node["parameter"]) {
        if (node["parameter"].IsScalar()) {
            if (isValidUUID4(node["parameter"].as<string>())) {
                boost::uuids::uuid paramId = boost::lexical_cast<boost::uuids::uuid>(node["parameter"].as<string>());

                parseNowOrLater(paramId, el->uuid, &ParameterNodeParser::setParameterLater);
            }
        }
    }
    
    return ret;
}

Element* ParameterNodeParser::createElement() {
    theEl = new ParameterNode;
    return theEl;
}

bool ParameterNodeParser::emit(YAML::Emitter& emitter, Element* el) {
    return true;
}