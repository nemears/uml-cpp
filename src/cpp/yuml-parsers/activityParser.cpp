#include "yuml-parsers/activityParser.h"

using namespace UML;

Element* ActivityParser::createElement() {
    theEl = new Activity;
    return theEl;
}

bool ActivityParser::parseFeatures(YAML::Node node, Element* el) {
    return ClassParser::parseFeatures(node, el);
}

bool ActivityParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::ACTIVITY) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "activity";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = ClassParser::emit(emitter, el);

    if (el->getElementType() == ElementType::ACTIVITY) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}