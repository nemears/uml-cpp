#include "yuml-parsers/objectFlowParser.h"

using namespace UML;

Element* ObjectFlowParser::createElement() {
    theEl = new ObjectFlow;
    return theEl;
}

bool ObjectFlowParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::OBJECT_FLOW) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "objectFlow";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = ActivityEdgeParser::emit(emitter, el);

    if (el->getElementType() == ElementType::OBJECT_FLOW) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

ObjectFlowParser ObjectFlowParser::createNewParser() {
    return ObjectFlowParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}