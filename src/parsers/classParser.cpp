#include "classParser.h"

Element* ClassParser::createElement() {
    return new Class;
}

bool ClassParser::parseTag(InputParser* input, Element* el) {
    return NamedElementParser::parseTag(input,el);
}