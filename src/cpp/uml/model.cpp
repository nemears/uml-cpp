#include "uml/model.h"

using namespace UML;

ElementType Model::getElementType() const {
    return ElementType::MODEL;
}

bool Model::isSubClassOf(ElementType eType) const {
    bool ret = Package::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MODEL;
    }

    return ret;
}