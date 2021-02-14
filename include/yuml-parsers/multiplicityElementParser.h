#ifndef MULTIPLICITYPARSER_H
#define MULTIPLICITYPARSER_H

#include "elementParser.h"
#include "uml/multiplicityElement.h"

class MultiplicityElementParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el);
    public:
        bool emit(YAML::Emitter& emitter, Element* el);
        bool parseMultiplicityFeatures(YAML::Node node, Element* el);
};

#endif