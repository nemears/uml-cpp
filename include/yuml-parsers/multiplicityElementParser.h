#ifndef MULTIPLICITYPARSER_H
#define MULTIPLICITYPARSER_H

#include "elementParser.h"
#include "uml/multiplicityElement.h"

class MultiplicityElementParser : public ElementParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el) override;
    public:
        MultiplicityElementParser(map<boost::uuids::uuid, UML::Element*>* elements) : ElementParser(elements){
            
        };

        bool emit(YAML::Emitter& emitter, Element* el) override;
};

#endif