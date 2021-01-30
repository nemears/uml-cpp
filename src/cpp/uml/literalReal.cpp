#include "uml/literalReal.h"

using namespace UML;

double LiteralReal::getValue() {
    return value;
}

void LiteralReal::setValue(double val) {
    value = val;
}

ElementType LiteralReal::getElementType() {
    return ElementType::LITERAL_REAL;
}