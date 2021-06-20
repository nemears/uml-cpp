#include "uml/type.h"

using namespace UML;

ElementType Type::getElementType() const {
    return ElementType::TYPE;
}

bool Type::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TYPE;
    }

    return ret;
}