#ifndef NAMEDELEMENTPARSER
#define NAMEDELEMENTPARSER
#include "../../uml/headers/namedElement.h"
#include "elementParser.h"
using namespace std;

class NamedElementParser : public ElementParser {
    protected:
        bool parseTag(InputParser* inputParser, Element * el);
    public:
        NamedElementParser(map<boost::uuids::uuid, Element*>* elements) : ElementParser(elements){
            //this->elements = elements;
        };
};

#endif