#include "yuml-parsers/behaviorParser.h"

bool BehaviorParser::parseFeatures(YAML::Node node, Element* el) {

    bool ret = ClassParser::parseFeatures(node, el);

    if (node["parameters"]) {
        if (node["parameters"].IsSequence()) {
            for (std::size_t i=0; i<node["parameters"].size(); i++) {
                if (node["parameters"][i].IsMap()) {
                    ParameterParser pp(elements, postProcessFlag);
                    Element* parsedEl = pp.TypedElementParser::parseElement(node["parameters"][i]);
                    dynamic_cast<Behavior*>(el)->parameters.push_back(dynamic_cast<Parameter*>(parsedEl));
                } else {
                    if (node["parameters"][i].IsScalar()) {
                        if (isValidUUID4(node["parameters"][i].as<string>())) {
                            boost::uuids::uuid paramId = boost::lexical_cast<boost::uuids::uuid>(node["parameters"][i].as<string>());
                            parseNowOrLater(paramId, el->uuid, &BehaviorParser::addParameterLater);
                        } else {
                            throw el->invalidID_Exception;
                        }
                    } else {
                        throw ElementParser::InvalidNodeTypeException(node["parameters"][i].Mark().line, "map or scalar");
                    }
                }
            }
        } else {
            // error
            throw ElementParser::InvalidNodeTypeException(node["parameters"].Mark().line, "sequence");
        }
    }

    return ret;
}

bool BehaviorParser::emit(YAML::Emitter& emitter, Element* el) {
    return true;
}