#include "namespaceParser.h"

using namespace std;

bool NamespaceParser::parseTag(InputParser* input, Element* el) {
    return NamedElementParser::parseTag(input, el);
}