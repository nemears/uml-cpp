#ifndef TYPEDELEMENTPARSER
#define TYPEDELEMENTPARSER
#include "namedElementParser.h"
#include "../../uml/headers/typedElement.h"

class TypedElementParser : public NamedElementParser {
    protected:
        bool parseFeatures(YAML::Node, UML::Element* el);

    public:
        TypedElementParser(map<boost::uuids::uuid, UML::Element*>* elements) : NamedElementParser(elements){
            // this->elements = elements;
        };
};
#endif