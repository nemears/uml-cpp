#include "classifierParser.h"

bool ClassifierParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("attributes") == 0) {
        int numTabs = input->numTabs;
        if (input->readNextLine()) {

            // parse within attributes scope
            while (input->numTabs >= numTabs) {
                if (input->firstWord.compare("property") == 0) {
                    PropertyParser propertyParser;
                    Property* createdEl = (Property*) propertyParser.parseElement(input);
                    ((Classifier*)el)->getOwnedAttributes().push_back(createdEl);
                }
            }
            return true;
        }
    }
    return NamedElementParser::parseTag(input, el);
}