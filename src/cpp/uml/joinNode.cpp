#include "uml/joinNode.h"

using namespace UML;

ElementType JoinNode::getElementType() const {
    return ElementType::JOIN_NODE;
}

bool JoinNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::JOIN_NODE;
    }

    return ret;
}