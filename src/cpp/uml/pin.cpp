#include "uml/pin.h"

using namespace UML;

ElementType Pin::getElementType() const {
    return ElementType::PIN;
}

bool Pin::isSubClassOf(ElementType eType) const {
    bool ret = ObjectNode::isSubClassOf(eType);

    if (!ret) {
        ret = MultiplicityElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PIN;
    }

    return ret;
}

void Pin::referencingReleased(ID id) {
    TypedElement::referencingReleased(id);
    MultiplicityElement::referencingReleased(id);
}