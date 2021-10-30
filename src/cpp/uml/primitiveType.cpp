#include "uml/primitiveType.h"

using namespace UML;

PrimitiveType::PrimitiveType() : Element(ElementType::PRIMITIVE_TYPE) {}

PrimitiveType::PrimitiveType(const PrimitiveType& rhs) : Element(rhs, ElementType::PRIMITIVE_TYPE) {}

bool PrimitiveType::isSubClassOf(ElementType eType) const {
    bool ret = DataType::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PRIMITIVE_TYPE;
    }

    return ret;
}