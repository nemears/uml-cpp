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

bool ElementParser::parseFeatures(YAML::Node node, Element* el) {
    if (node["id"]) {
        boost::uuids::uuid oldId = el->uuid;
        try {
            el->setID(node["id"].as<string>());

            // override elements entry
            elements->erase(oldId);
            (*elements)[el->uuid] = el;
        } catch (exception& e) {
            cerr << e.what() << '\n';
        }
    } 
    if (node["children"]) {
        // TODO
        if (node["children"].IsSequence()) {
            for (std::size_t i=0; i<node["children"].size(); i++) {
                if (node["children"][i]["class"]) {
                    ClassParser classParser(elements);
                    Element* parsedEl = classParser.parseElement(node["children"][i]["class"]);
                    el->ownedElements.push_back(parsedEl);
                } else if (node["children"][i]["other types here"]) {
                    // TODO TODO TODO
                }
            }
        } else {
            // ERROR
        }
    }
    return true;
}