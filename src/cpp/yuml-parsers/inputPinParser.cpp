#include "yuml-parsers/inputPinParser.h"

Element* InputPinParser::createElement() {
    TypedElementParser::theEl = new InputPin;
    return TypedElementParser::theEl;
}

InputPinParser InputPinParser::createNewParser() {
    return InputPinParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}