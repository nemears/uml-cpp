#ifndef ELEMENTPARSER
#define ELEMENTPARSER
#include "templateParser.h"
using namespace std;

class ElementParser : public TemplateParser {
    protected:
        Element parseElement(InputParser* inputParser);
        bool parseTag(InputParser* inputParser, Element * el);
};
#endif