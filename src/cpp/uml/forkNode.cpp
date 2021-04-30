#include "uml/forkNode.h"

using namespace UML;

ElementType ForkNode::getElementType() const {
    return ElementType::FORK_NODE;
}

bool ForkNode::isSubClassOf(ElementType eType) {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::FORK_NODE;
    }

    return ret;
}