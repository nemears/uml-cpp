#include "headers/propertyParser.h"

Element* PropertyParser::createElement(){
    return new Property;
}

bool PropertyParser::parseTag(InputParser* input, Element* el) {

    if (input->firstWord.compare("defaultValue")) {
        // TODO process default value of property
    }

    return TypedElementParser::parseTag(input, el);
}