#ifndef ELEMENTPARSER
#define ELEMENTPARSER
#include <list>
#include <iostream>
#include "templateParser.h"
using namespace std;

class ElementParser : public TemplateParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el);
    public:
        ElementParser(map<boost::uuids::uuid, Element*>* elements) : TemplateParser(elements) {
            
        };
};
#endif