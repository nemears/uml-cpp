#include "uml/literalString.h"

using namespace UML;

string LiteralString::getValue() {
    return value;
}

void LiteralString::setValue(string val) {
    value = val;
}