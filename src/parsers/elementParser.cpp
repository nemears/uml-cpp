#include "elementParser.h"

bool ElementParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("id") == 0) {

        el->setId(input->getTag());

        return true;
    } else if (input->firstWord.compare("children")) {

        // TODO   

    }

    return false;
}