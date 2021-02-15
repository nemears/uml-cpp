#include "yuml-parsers/inputPinParser.h"

Element* InputPinParser::createElement() {
    TypedElementParser::theEl = new InputPin;
    return TypedElementParser::theEl;
}