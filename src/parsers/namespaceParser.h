#ifndef NAMESPACEPARSER
#define NAMESPACEPARSER

#include "elementParser.h"
#include "../uml/namespace.h"

class NamespaceParser : public ElementParser {
    protected:
    Element parseElement(InputParser* inputParser);
    bool parseTag(InputParser* inputParser, Element * el);
};

#endif