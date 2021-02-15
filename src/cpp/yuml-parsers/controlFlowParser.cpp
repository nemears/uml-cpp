#include "yuml-parsers/controlFlowParser.h"

using namespace UML;

Element* ControlFlowParser::createElement() {
    theEl = new ControlFlow;
    return theEl;
}