#include "uml/operation.h"

using namespace UML;

Type* Operation::getType() {
    return type;
}

void Operation::setType(Type* type) {
    this->type = type;
}