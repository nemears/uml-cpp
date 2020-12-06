#include "headers/elementParser.h"
#include "headers/classParser.h"

bool ElementParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("id") == 0) {

        boost::uuids::uuid oldId = el->uuid;
        try {
            el->setID(input->getTag());

            // override elements entry
            elements->erase(oldId);
            (*elements)[el->uuid] = el;
        } catch (exception& e) {
            cerr << e.what() << '\n';
        }

        return true;
    } else if (input->firstWord.compare("children") == 0) {
        int numTabs = input->numTabs;
        while(input->readNextLine()) {

            // parse within children scope
            if (input->numTabs >= numTabs) {
                if (input->firstWord.compare("class") == 0) {
                    ClassParser classParser(elements);
                    Element* parsedEl = classParser.parseElement(input);
                    el->ownedElements.push_back(parsedEl);
                    if (input->nextLineTabs <= numTabs) {
                        break;
                    }
                } 
                // this is if it cannot find any valid option
                else {
                    cerr << "could not find any suitable keyword after children scope, File: " << input->getFileName() << ", line number " << input->lineNumber << '\n';
                    if (input->numTabs <= numTabs){
                        input->resetLine();
                    }
                    return true;
                }
            } else {
                break;
            }
        }
        return true;
    }

    return false;
}