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

            parseNowOrLater(behaviorId, el->uuid, &CallBehaviorActionParser::setBehaviorLater);
        } else if (node["behavior"].IsMap()) {
            // TODO
        }
    }

    return ret;
}

bool CallBehaviorActionParser::emit(YAML::Emitter& emitter, Element* el) {
    return true;
}