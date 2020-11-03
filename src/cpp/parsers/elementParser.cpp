#include "headers/elementParser.h"
#include "headers/classParser.h"

bool ElementParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("id") == 0) {

        el->setID(input->getTag());

        return true;
    } else if (input->firstWord.compare("children") == 0) {
        int numTabs = input->numTabs;
        while(input->readNextLine()) {

            // parse within children scope
            if (input->numTabs >= numTabs) {
                if (input->firstWord.compare("class") == 0) {
                    ClassParser classParser;
                    Element* parsedEl = classParser.parseElement(input);
                    el->getOwnedElements().push_back(parsedEl);
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

    return false;
}