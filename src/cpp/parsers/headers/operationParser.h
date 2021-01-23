#ifndef OPERATIONPARSER
#define OPERATIONPARSER

#include "namedElementParser.h"

using namespace UML;

class OperationParser : public NamedElementParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        OperationParser(map<boost::uuids::uuid, UML::Element*>* elements) : NamedElementParser(elements){
            // this->elements = elements;
        };
};

#endif