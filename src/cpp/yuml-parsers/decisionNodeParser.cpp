#include "yuml-parsers/decisionNodeParser.h"

Element* DecisionNodeParser::createElement() {
    theEl =  new DecisionNode;
    return theEl;
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