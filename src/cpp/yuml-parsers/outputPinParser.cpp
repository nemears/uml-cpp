#include "yuml-parsers/outputPinParser.h"

Element* OutputPinParser::createElement() {
    TypedElementParser::theEl = new OutputPin;
    return TypedElementParser::theEl;
}

OutputPinParser OutputPinParser::createNewParser() {
    return OutputPinParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}