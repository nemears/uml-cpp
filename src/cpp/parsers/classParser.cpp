#include "headers/classParser.h"

UML::Element* ClassParser::createElement() {
    return new UML::Class;
}

bool ClassParser::parseFeatures(YAML::Node node, UML::Element* el) {

    if (node["operations"]) {
        if (node["operations"].IsSequence()) {
            for (std::size_t i=0; i<node["operations"].size(); i++) {
                // TODO operations parser
            }
        }
    }

    return ClassifierParser::parseFeatures(node, el);
}