#include "yuml-parsers/controlFlowParser.h"

using namespace UML;

Element* ControlFlowParser::createElement() {
    theEl = new ControlFlow;
    return theEl;
}

ControlFlowParser ControlFlowParser::createNewParser() {
    return ControlFlowParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}