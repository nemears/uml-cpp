#ifndef PROPERTYPARSER
#define PROPERTYPARSER
#include "typedElementParser.h"
#include "../../uml/headers/property.h"
#include "../../uml/headers/primitiveType.h"
#include "../../uml/headers/literalString.h"
#include "../../uml/headers/literalInt.h"
#include "../../uml/headers/literalReal.h"
#include "../../uml/headers/literalBool.h"
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
};
#endif