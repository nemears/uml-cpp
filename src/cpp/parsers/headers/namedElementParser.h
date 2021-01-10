#ifndef NAMEDELEMENTPARSER
#define NAMEDELEMENTPARSER
#include "../../uml/headers/namedElement.h"
#include "elementParser.h"
using namespace std;

class NamedElementParser : public ElementParser {
    protected:
        virtual bool parseFeatures(YAML::Node node, Element* el);
    public:
        NamedElementParser(map<boost::uuids::uuid, Element*>* elements) : ElementParser(elements){
            //this->elements = elements;
        };
};

#endif