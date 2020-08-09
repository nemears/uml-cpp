#include "namespaceParser.h"

bool NamespaceParser::parseTag(InputParser* input, Element* el) {
    return NamedElementParser::parseTag(input, el);
}