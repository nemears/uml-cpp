#include "headers/primitiveType.h"

using namespace UML;

void PrimitiveType::setPrimitiveType(Primitive primitive) {
    this->primitiveType = primitive;
}

PrimitiveType::Primitive PrimitiveType::getPrimitiveType() {
    return this->primitiveType;
}

bool PrimitiveType::isPrimitive() {
    return true;
}