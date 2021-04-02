#include "uml/joinNode.h"

using namespace UML;

ElementType JoinNode::getElementType() {
    return ElementType::JOIN_NODE;
}

bool JoinNode::isSubClassOf(ElementType eType) {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::JOIN_NODE;
    }

    return ret;
}