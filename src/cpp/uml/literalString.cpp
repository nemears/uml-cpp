#include "uml/literalString.h"

using namespace UML;

string LiteralString::getValue() {
    return value;
}

void LiteralString::setValue(string val) {
    value = val;
}

ElementType LiteralString::getElementType() {
    return ElementType::LITERAL_STRING;
}