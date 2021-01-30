#include "uml/type.h"

using namespace UML;

bool Type::isPrimitive() {
    return false;
}

ElementType Type::getElementType() {
    return ElementType::TYPE;
}