#include "yuml-parsers/behaviorParser.h"

bool BehaviorParser::parseFeatures(YAML::Node node, Element* el) {

    bool ret = ClassParser::parseFeatures(node, el);

    if (node["parameters"]) {
        if (node["parameters"].IsSequence()) {
            for (std::size_t i=0; i<node["parameters"].size(); i++) {
                if (node["parameters"][i].IsMap()) {
                    if (node["parameters"][i]["parameter"]) {
                        ParameterParser pp(elements, postProcessFlag);
                        Element* parsedEl = pp.TypedElementParser::parseElement(node["parameters"][i]["parameter"]);
                        dynamic_cast<Behavior*>(el)->getParameters().add(*dynamic_cast<Parameter*>(parsedEl));
                    } else {
                        // error
                        YAML::Emitter errEmit;
                        errEmit << node["parameters"][i];
                        throw ElementParser::InvalidIdentifierException(node["parameters"][i].Mark().line, errEmit.c_str());
                    }
                } else {
                    if (node["parameters"][i].IsScalar()) {
                        if (isValidUUID4(node["parameters"][i].as<string>())) {
                            boost::uuids::uuid paramId = boost::lexical_cast<boost::uuids::uuid>(node["parameters"][i].as<string>());
                            parseNowOrLater(paramId, el->getID(), node, &BehaviorParser::addParameterLater);
                        } else {
                            throw InvalidID_Exception();
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
    bool ret = ClassParser::emit(emitter, el);

    if(!dynamic_cast<Behavior*>(el)->getParameters().empty()) {
        emitter << YAML::Key << "parameters";
        emitter << YAML::Value << YAML::BeginSeq;
        for(auto const& param : dynamic_cast<Behavior*>(el)->getParameters().iterator()) {
            // TODO don't conflict with operation paired parameter
            ParameterParser pp (elements, postProcessFlag);
            pp.emit(emitter, param);
        }
        emitter << YAML::EndSeq;
    }
    
    return ret;
}