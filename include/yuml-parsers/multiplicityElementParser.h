#ifndef MULTIPLICITYPARSER_H
#define MULTIPLICITYPARSER_H

#include "elementParser.h"
#include "uml/multiplicityElement.h"

class MultiplicityElementParser : virtual public ElementParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el) override;
        bool parseMultiplicityFeatures(YAML::Node node, Element* el);
        bool emitMultiplicity(YAML::Emitter& emitter, Element* el);
    public:
        bool emit(YAML::Emitter& emitter, Element* el) override;
        MultiplicityElementParser(map<boost::uuids::uuid, Element*>* elements) : ElementParser(elements) {};
};

#endif