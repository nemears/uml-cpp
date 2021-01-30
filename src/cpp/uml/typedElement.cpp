#include "uml/typedElement.h"

using namespace UML;

Type* TypedElement::getType() {
    return type;
}

void TypedElement::setType(Type* type) {
    this->type = type;
}

ElementType TypedElement::getElementType() {
    return ElementType::TYPED_ELEMENT;
}