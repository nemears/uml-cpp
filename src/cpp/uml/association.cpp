#include "uml/association.h"

using namespace UML;

ElementType Association::getElementType() {
    return ElementType::ASSOCIATION;
}

bool Association::isSubClassOf(ElementType eType) {
    bool ret = Relationship::isSubClassOf(eType);

    if (!ret) {
        ret = Classifier::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::ASSOCIATION;
    }

    return ret;
}