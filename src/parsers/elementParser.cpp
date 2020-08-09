#include "elementParser.h"
#include "classParser.h"

bool ElementParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("id") == 0) {

        el->setId(input->getTag());

        return true;
    } else if (input->firstWord.compare("children") == 0) {
        if (input->readNextLine()) {
            if (input->firstWord.compare("class") == 0) {
                ClassParser classParser;
                Element* parsedEl = classParser.parseElement(input);
                el->getOwnedElements().push_back(parsedEl);
                return true;
            }
        }
    }

    return false;
}