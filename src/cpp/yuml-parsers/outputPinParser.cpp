#include "yuml-parsers/outputPinParser.h"

Element* OutputPinParser::createElement() {
    TypedElementParser::theEl = new OutputPin;
    return TypedElementParser::theEl;
}