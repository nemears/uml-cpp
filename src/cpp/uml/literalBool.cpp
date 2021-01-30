#include "uml/literalBool.h"

using namespace UML;

bool LiteralBool::getValue() {
    return value;
}

void LiteralBool::setValue(bool val) {
    value = val;
}

ElementType LiteralBool::getElementType() {
    return ElementType::LITERAL_BOOL;
}