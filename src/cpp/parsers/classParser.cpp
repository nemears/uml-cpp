#include "headers/classParser.h"

Element* ClassParser::createElement() {
    return new Class;
}

bool ClassParser::parseTag(InputParser* input, Element* el) {
    return ClassifierParser::parseTag(input,el);
}