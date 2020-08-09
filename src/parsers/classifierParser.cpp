#include "classifierParser.h"

bool ClassifierParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("attributes") == 0) {
        int numTabs = input->numTabs;
        if (input->readNextLine()) {

            // parse within attributes scope
            while (input->numTabs >= numTabs) {
                
            }
            return false; // TODO change to true
        }
    }
    return NamedElementParser::parseTag(input, el);
}