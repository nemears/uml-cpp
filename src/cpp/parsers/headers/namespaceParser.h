#ifndef NAMESPACEPARSER
#define NAMESPACEPARSER

#include "namedElementParser.h"
#include "../../uml/headers/namespace.h"

class NamespaceParser : public NamedElementParser {
    protected:
        bool parseTag(InputParser* inputParser, Element * el);
    public:
        NamespaceParser(map<boost::uuids::uuid, Element*>* elements) : NamedElementParser(elements){
            //this->elements = elements;
        };
};

#endif