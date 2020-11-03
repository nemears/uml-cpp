#ifndef ELEMENTPARSER
#define ELEMENTPARSER
#include <list>
#include "templateParser.h"
using namespace std;

class ElementParser : public TemplateParser {
    protected:
        bool parseTag(InputParser* inputParser, Element * el);
};
#endif