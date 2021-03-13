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
#include "expressionParser.h"

using namespace UML;

class SlotParser : public ElementParser {
    protected:
        UML::Element* createElement();
        bool parseFeatures(YAML::Node node, UML::Element* el);
        void parseNonPimitiveValueFeatures(YAML::Node node, Element* el);

    public:
        SlotParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ElementParser(elements, postParsers){
            this->keyword = "slot";
        }
        bool emit(YAML::Emitter& emitter, Element* el);
        static SlotParser createNewParser();
        static void setDefiningFeatureLater(YAML::Node node, Element* slot, Element* definingFeature) {
            dynamic_cast<Slot*>(slot)->setDefiningFeature(dynamic_cast<StructuralFeature*>(definingFeature));
        }
        static void setInstanceValueLater(YAML::Node node, Element* slot, Element* value) {
            if (((Slot*) slot)->getDefiningFeature() == NULL) {

                /**
                 * TODO uncomment the line below this blurb, So... the value list in slot
                 * can't throw an error if added to so i want to do it here as if it was in there
                 **/

                //throw ((Slot*)slot)->nullDefiningFeatureException;
            }
            InstanceValue* iv = new InstanceValue;
            iv->setInstance(dynamic_cast<InstanceSpecification*>(value));
            dynamic_cast<Slot*>(slot)->getValues().add(*iv);
        }
};

#endif