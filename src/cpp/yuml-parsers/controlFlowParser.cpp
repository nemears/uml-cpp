#include "yuml-parsers/controlFlowParser.h"

using namespace UML;

Element* ControlFlowParser::createElement() {
    theEl = new ControlFlow;
    return theEl;
}

bool ControlFlowParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::CONTROL_FLOW) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "controlFlow";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = ActivityEdgeParser::emit(emitter, el);

    if (el->getElementType() == ElementType::CONTROL_FLOW) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

ControlFlowParser ControlFlowParser::createNewParser() {
    return ControlFlowParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}