#ifndef ELEMENTPARSER
#define ELEMENTPARSER
#include <list>
#include <iostream>
#include "templateParser.h"
using namespace std;

class ElementParser : public TemplateParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el);
    public:
        ElementParser(map<boost::uuids::uuid, UML::Element*>* elements) : TemplateParser(elements) {
            
        };
};
#endif