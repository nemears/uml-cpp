#ifndef PROPERTYPARSER
#define PROPERTYPARSER
#include "typedElementParser.h"
#include "uml/property.h"
#include "uml/primitiveType.h"
#include "uml/literalString.h"
#include "uml/literalInt.h"
#include "uml/literalReal.h"
#include "uml/literalBool.h"
#include "uml/instanceSpecification.h"
#include "uml/instanceValue.h"

using namespace std;

class PropertyParser : public TypedElementParser {
    protected:
        UML::Element* createElement();
        bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        PropertyParser(map<boost::uuids::uuid, UML::Element*>* elements) : TypedElementParser(elements){
            this->keyword = "property";
            // this->elements = elements;
        }

        bool emit(YAML::Emitter& emitter, Element* el);
};
#endif