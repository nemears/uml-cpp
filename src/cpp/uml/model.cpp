#include "uml/model.h"

using namespace UML;

ElementType Model::getElementType() {
    return ElementType::MODEL;
}

bool Model::isSubClassOf(ElementType eType) {
    bool ret = Namespace::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MODEL;
    }

    return ret;
}