#include "uml/objectNode.h"

using namespace UML;

ValueSpecification* ObjectNode::getUpperBound() {
    return upperBound;
}

void ObjectNode::setUpperBound(ValueSpecification* upperBound) {
    this->upperBound = upperBound;
}

ElementType ObjectNode::getElementType() {
    return ElementType::OBJECT_NODE;
}

bool ObjectNode::isObjectNode() {
    return true;
}