#ifndef NAMESPACEPARSER
#define NAMESPACEPARSER

#include "templateParser.h"
#include "../uml/namespace.h"

class NamespaceParser : public TemplateParser {
    protected:
    Element parseElement(InputParser* inputParser);
    bool parseTag(InputParser* inputParser, Namespace el);
};

#endif