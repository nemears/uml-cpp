#include "yuml-parsers/objectFlowParser.h"

using namespace UML;

Element* ObjectFlowParser::createElement() {
    theEl = new ObjectFlow;
    return theEl;
}