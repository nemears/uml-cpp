#ifndef PROPERTYPARSER
#define PROPERTYPARSER
#include "namedElementParser.h"
#include "../../uml/headers/property.h"
using namespace std;

class PropertyParser : public NamedElementParser {
    protected:
        Element* createElement();
        bool parseTag(InputParser* input, Element * el);

    public:
        PropertyParser() {
            this->keyword = "property";
        }
};
#endif