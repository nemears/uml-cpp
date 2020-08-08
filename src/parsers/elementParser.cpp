#include "elementParser.h"

bool ElementParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("id") == 0) {

        el->setId(input->getTag());

        return true;
    }

    return false;
}