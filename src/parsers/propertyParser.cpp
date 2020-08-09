#include "propertyParser.h"

Element* PropertyParser::createElement(){
    return new Property;
}

bool PropertyParser::parseTag(InputParser* input, Element* el) {
    return NamedElementParser::parseTag(input, el);
}