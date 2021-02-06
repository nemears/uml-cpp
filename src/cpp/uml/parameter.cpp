#include "uml/parameter.h"

using namespace UML;

ElementType Parameter::getElementType() {
    return ElementType::PARAMETER;
}

ParameterDirectionKind Parameter::getDirection() {
    return direction;
}

void Parameter::setDirection(ParameterDirectionKind direction) {
    this->direction = direction;
}