#ifndef NAMEDELEMENTPARSER
#define NAMEDELEMENTPARSER
#include "../uml/namedElement.h"
#include "elementParser.h"
using namespace std;

class NamedElementParser : public ElementParser {
    protected:
        bool parseTag(InputParser* inputParser, Element * el);
};

#endif