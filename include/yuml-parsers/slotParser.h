#ifndef SLOTPARSER
#define SLOTPARSER

#include "elementParser.h"
#include "uml/slot.h"
#include "uml/primitiveType.h"
#include "uml/literalString.h"
#include "uml/literalInt.h"
#include "uml/literalReal.h"
#include "uml/literalBool.h"
#include "uml/instanceSpecification.h"
#include "uml/instanceValue.h"

using namespace UML;

class SlotParser : public ElementParser {
    protected:
        UML::Element* createElement();
        bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        SlotParser(map<boost::uuids::uuid, UML::Element*>* elements) : ElementParser(elements){
            this->keyword = "slot";
        }
        bool emit(YAML::Emitter& emitter, Element* el);
};

#endif