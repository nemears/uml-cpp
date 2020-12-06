#include "headers/primitiveType.h"

void PrimitiveType::setPrimitiveType(Primitive primitive) {
    this->primitiveType = primitive;
}

PrimitiveType::Primitive PrimitiveType::getPrimitiveType() {
    return this->primitiveType;
}