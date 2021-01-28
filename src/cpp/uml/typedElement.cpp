#include "uml/typedElement.h"

UML::Type* UML::TypedElement::getType() {
    return type;
}

void UML::TypedElement::setType(Type* type) {
    this->type = type;
}