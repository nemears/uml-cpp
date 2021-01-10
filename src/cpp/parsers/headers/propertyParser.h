#ifndef PROPERTYPARSER
#define PROPERTYPARSER
#include "typedElementParser.h"
#include "../../uml/headers/property.h"
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