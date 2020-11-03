#include "headers/typedElementParser.h"

bool TypedElementParser::parseTag(InputParser * input, Element * el) {
    if (input->firstWord.compare("type") == 0) {

        // TODO check if overiding

        // TODO get element from Map???

        //((TypedElement*)el)->setType(/*TODO*/)

    }
    return false;
}