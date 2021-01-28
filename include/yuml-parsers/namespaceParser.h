#ifndef NAMESPACEPARSER
#define NAMESPACEPARSER

#include "namedElementParser.h"
#include "uml/namespace.h"

class NamespaceParser : public NamedElementParser {
    protected:
        virtual bool parseFeatures(YAML::Node node, UML::Element* el);
    public:
        NamespaceParser(map<boost::uuids::uuid, UML::Element*>* elements) : NamedElementParser(elements){
            //this->elements = elements;
        };
};

#endif