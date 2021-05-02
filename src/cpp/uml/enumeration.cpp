#include "uml/enumeration.h"

using namespace UML;

ElementType Enumeration::getElementType() const {
    return ElementType::ENUMERATION;
}

bool Enumeration::isSubClassOf(ElementType eType) {
    bool ret = DataType::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION;
    }

    return ret;
}