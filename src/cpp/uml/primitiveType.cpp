#include "headers/primitiveType.h"

void UML::PrimitiveType::setPrimitiveType(Primitive primitive) {
    this->primitiveType = primitive;
}

UML::PrimitiveType::Primitive UML::PrimitiveType::getPrimitiveType() {
    return this->primitiveType;
}