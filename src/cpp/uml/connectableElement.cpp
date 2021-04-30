#include "uml/connectableElement.h"

using namespace UML;

ElementType ConnectableElement::getElementType() const {
    return ElementType::CONNECTABLE_ELEMENT;
}

bool ConnectableElement::isSubClassOf(ElementType eType) {
    bool ret = TypedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CONNECTABLE_ELEMENT;
    }

    return ret;
}