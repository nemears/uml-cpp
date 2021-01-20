#ifndef SLOTPARSER
#define SLOTPARSER

#include "elementParser.h"
#include "../uml/headers/slot.h"
#include "../uml/headers/primitiveType.h"
#include "../../uml/headers/literalString.h"
#include "../../uml/headers/literalInt.h"
#include "../../uml/headers/literalReal.h"
#include "../../uml/headers/literalBool.h"
#include "../../uml/headers/instanceSpecification.h"

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