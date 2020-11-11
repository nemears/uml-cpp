#ifndef ELEMENTPARSER
#define ELEMENTPARSER
#include <list>
#include "templateParser.h"
using namespace std;

class ElementParser : public TemplateParser {
    protected:
        bool parseTag(InputParser* inputParser, Element * el);
    public:
        ElementParser(map<boost::uuids::uuid, Element*>* elements) : TemplateParser(elements) {
            //this->elements = elements;
        };
};
#endif