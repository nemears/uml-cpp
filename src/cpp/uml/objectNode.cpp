#include "uml/objectNode.h"

using namespace UML;

void ObjectNode::restoreReleased(ID id, Element* released) {
    ActivityNode::restoreReleased(id, released);
    TypedElement::restoreReleased(id, released);
}

void ObjectNode::referencingReleased(ID id) {
    ActivityNode::referencingReleased(id);
    TypedElement::referencingReleased(id);
}

ValueSpecification* ObjectNode::getUpperBound() {
    return upperBound;
}

void ObjectNode::setUpperBound(ValueSpecification* upperBound) {
    this->upperBound = upperBound;
}

ElementType ObjectNode::getElementType() const {
    return ElementType::OBJECT_NODE;
}

bool ObjectNode::isObjectNode() {
    return true;
}

bool ObjectNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = TypedElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::OBJECT_NODE;
    }

    return ret;
}