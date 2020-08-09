#include "modelParser.h"

Element* ModelParser::createElement() {
    return new Model;
}

bool ModelParser::parseTag(InputParser* input, Element* el) {
    return NamespaceParser::parseTag(input, el);
}