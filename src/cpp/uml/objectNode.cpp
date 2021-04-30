#include "uml/objectNode.h"

using namespace UML;

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

bool ObjectNode::isSubClassOf(ElementType eType) {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = TypedElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::OBJECT_NODE;
    }

    return ret;
}