#ifndef PROPERTYPARSER
#define PROPERTYPARSER
#include "typedElementParser.h"
#include "../../uml/headers/property.h"
using namespace std;

class PropertyParser : public TypedElementParser {
    protected:
        Element* createElement();
        bool parseTag(InputParser* input, Element * el);

    public:
        PropertyParser(map<boost::uuids::uuid, Element*>* elements) : TypedElementParser(elements){
            this->keyword = "property";
            // this->elements = elements;
        }
};
#endif