#include "headers/classifierParser.h"

bool ClassifierParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("attributes") == 0) {
        int numTabs = input->numTabs;
        while (input->readNextLine()) {

            // parse within attributes scope
            if (input->numTabs >= numTabs) {
                if (input->firstWord.compare("property") == 0) {
                    PropertyParser propertyParser(elements);
                    Property* createdEl = (Property*) propertyParser.parseElement(input);
                    ((Classifier*)el)->ownedAttributes.push_back(createdEl);
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

bool ClassifierParser::parseFeatures(YAML::Node node, Element* el) {
    if (node["attributes"]) {
        if (node["attributes"].IsSequence()) {
            for (std::size_t i=0; i<node["attributes"].size(); i++) {
                if (node["attributes"][i]["property"]) {
                    PropertyParser propertyParser(elements);
                    Element* parsedEl = propertyParser.parseElement(node["attributes"][i]["property"]);
                    el->ownedElements.push_back(parsedEl);
                } else if (node["attributes"][i]["other types here"]) {
                    // TODO TODO TODO
                }
            }
        } else {
            // ERROR
        }
    }
    return NamedElementParser::parseFeatures(node, el);
}