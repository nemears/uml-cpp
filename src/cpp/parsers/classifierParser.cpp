#include "headers/classifierParser.h"

bool ClassifierParser::parseFeatures(YAML::Node node, Element* el) {
    if (node["attributes"]) {
        if (node["attributes"].IsSequence()) {
            for (std::size_t i=0; i<node["attributes"].size(); i++) {
                if (node["attributes"][i]["property"]) {
                    PropertyParser propertyParser(elements);
                    Element* parsedEl = propertyParser.parseElement(node["attributes"][i]["property"]);
                    ((Classifier*) el)->ownedAttributes.push_back((Property*) parsedEl);
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