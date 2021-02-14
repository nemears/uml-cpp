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
#include "multiplicityElementParser.h"
using namespace std;

class PropertyParser : public TypedElementParser, public MultiplicityElementParser {
    protected:
        UML::Element* createElement();
        bool parseFeatures(YAML::Node node, UML::Element* el) override;

    public:
        PropertyParser(map<boost::uuids::uuid, UML::Element*>* elements) : TypedElementParser(elements) , MultiplicityElementParser(elements), NamedElementParser(elements), ElementParser(elements) {
            this->TypedElementParser::keyword = "property";
            // this->elements = elements;
        }

        bool emit(YAML::Emitter& emitter, Element* el) override;
};
#endif