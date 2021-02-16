#include "yuml-parsers/objectFlowParser.h"

using namespace UML;

Element* ObjectFlowParser::createElement() {
    theEl = new ObjectFlow;
    return theEl;
}

ObjectFlowParser ObjectFlowParser::createNewParser() {
    return ObjectFlowParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}