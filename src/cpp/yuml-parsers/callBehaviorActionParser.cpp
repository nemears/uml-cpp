#include "yuml-parsers/callBehaviorActionParser.h"

Element* CallBehaviorActionParser::createElement() {
    theEl = new CallBehaviorAction;
    return theEl;
}

bool CallBehaviorActionParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = ActionParser::parseFeatures(node, el);

    if (node["behavior"]) {
        if (node["behavior"].IsScalar()) {
            boost::uuids::uuid behaviorId = boost::lexical_cast<boost::uuids::uuid>(node["behavior"].as<string>());

            parseNowOrLater(behaviorId, el->uuid, node, &CallBehaviorActionParser::setBehaviorLater);
        } else if (node["behavior"].IsMap()) {
            // TODO
        }
    }

    return ret;
}

bool CallBehaviorActionParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::CALL_BEHAVIOR_ACTION) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "callBehaviorAction";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = ActionParser::emit(emitter, el);

    if (dynamic_cast<CallBehaviorAction*>(el)->getBehavior() != NULL) {
        emitter << YAML::Key << "behavior";
        emitter << YAML::Value << dynamic_cast<CallBehaviorAction*>(el)->getBehavior()->getIDstring();
    }

    if (el->getElementType() == ElementType::CALL_BEHAVIOR_ACTION) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}