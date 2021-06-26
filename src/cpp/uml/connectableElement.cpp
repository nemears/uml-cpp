#include "uml/connectableElement.h"

using namespace UML;

ElementType ConnectableElement::getElementType() const {
    return ElementType::CONNECTABLE_ELEMENT;
}

bool ConnectableElement::isSubClassOf(ElementType eType) const {
    bool ret = TypedElement::isSubClassOf(eType);

    if (!ret) {
        ret = ParameterableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::CONNECTABLE_ELEMENT;
    }

    return ret;
}