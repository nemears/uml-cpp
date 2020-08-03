#include "modelParser.h"

using namespace std;

Element ModelParser::createElement() {
    Model ret;
    return ret;
}

bool ModelParser::parseTag(InputParser* input, Model el) {
    return NamespaceParser::parseTag(input, el);
}