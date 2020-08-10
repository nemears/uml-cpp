#include "classifierParser.h"

bool ClassifierParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("attributes") == 0) {
        int numTabs = input->numTabs;
        while (input->readNextLine()) {

            // parse within attributes scope
            if (input->numTabs >= numTabs) {
                if (input->firstWord.compare("property") == 0) {
                    PropertyParser propertyParser;
                    Property* createdEl = (Property*) propertyParser.parseElement(input);
                    ((Classifier*)el)->getOwnedAttributes().push_back(createdEl);
                    if (input->nextLineTabs <= numTabs) {
                        break;
                    }
                }
            } else {
                break;
            }
        }
        return true;
    }
    return NamedElementParser::parseTag(input, el);
}