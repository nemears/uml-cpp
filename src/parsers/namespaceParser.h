#ifndef NAMESPACEPARSER
#define NAMESPACEPARSER

#include "namedElementParser.h"
#include "../uml/namespace.h"

class NamespaceParser : public NamedElementParser {
    protected:
    Element parseElement(InputParser* inputParser);
    bool parseTag(InputParser* inputParser, Element * el);
};

#endif