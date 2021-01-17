#ifndef SLOTPARSER
#define SLOTPARSER

#include "elementParser.h"

using namespace UML;

class SlotParser : public ElementParser {
    protected:
        UML::Element* createElement();
        bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        SlotParser(map<boost::uuids::uuid, UML::Element*>* elements) : ElementParser(elements){
            this->keyword = "slot";
            // this->elements = elements;
        }
};

#endif