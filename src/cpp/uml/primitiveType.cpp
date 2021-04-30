#include "uml/primitiveType.h"

using namespace UML;

ElementType PrimitiveType::getElementType() {
    return ElementType::PRIMITIVE_TYPE;
}

bool PrimitiveType::isSubClassOf(ElementType eType) {
    bool ret = DataType::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PRIMITIVE_TYPE;
    }

    return ret;
}