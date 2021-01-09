#ifndef TYPEDELEMENTPARSER
#define TYPEDELEMENTPARSER
#include "namedElementParser.h"
#include "../../uml/headers/typedElement.h"

class TypedElementParser : public NamedElementParser {
    protected:
        bool parseTag(InputParser* inputParser, Element * el);

        bool parseFeatures(YAML::Node, Element* el);

    public:
        TypedElementParser(map<boost::uuids::uuid, Element*>* elements) : NamedElementParser(elements){
            // this->elements = elements;
        };
};
#endif