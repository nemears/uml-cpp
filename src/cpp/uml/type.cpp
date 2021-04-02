#include "uml/type.h"

using namespace UML;

// bool Type::isPrimitive() {
//     return false;
// }

ElementType Type::getElementType() {
    return ElementType::TYPE;
}

bool Type::isSubClassOf(ElementType eType) {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TYPE;
    }

    return ret;
}