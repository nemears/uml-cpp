#include "uml/literalInt.h"

using namespace UML;

int LiteralInt::getValue() {
    return value;
}

void LiteralInt::setValue(int val) {
    value = val;
}

ElementType LiteralInt::getElementType() {
    return ElementType::LITERAL_INT;
}