#include "yuml-parsers/decisionNodeParser.h"

Element* DecisionNodeParser::createElement() {
    theEl =  new DecisionNode;
    return theEl;
}

bool DecisionNodeParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = ActivityNodeParser::parseFeatures(node, el);
    
    if (node["decisionInputFlow"]) {
        if (node["decisionInputFlow"].IsScalar()) {
            if (isValidUUID4(node["decisionInputFlow"].as<string>())) {
                boost::uuids::uuid decisionInputFlowID = boost::lexical_cast<boost::uuids::uuid>(node["decisionInputFlow"].as<string>());

                if(!parseNowOrLater(decisionInputFlowID, el->uuid, node, &DecisionNodeParser::parseDecisionInputFlowLater)) {
                    ObjectFlow* tempObjFlow = new ObjectFlow;
                    tempObjFlow->setID(node["decisionInputFlow"].as<string>());
                    dynamic_cast<DecisionNode*>(el)->setDecisionInputFlow(tempObjFlow);
                }
            } else {
                // decision input flow definition in body?
                // probably not, fatal error
                return false;
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["decisionInputFlow"].Mark().line, "sequence");
        }
    }

    return ret;
}

bool DecisionNodeParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::DECISION_NODE) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "decisionNode";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = NamedElementParser::emit(emitter, el);
    
    if (!emitActivityNode(emitter, el)) {
        ret = false;
    }

    if (el->getElementType() == ElementType::DECISION_NODE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

DecisionNodeParser DecisionNodeParser::createNewParser() {
    return DecisionNodeParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}