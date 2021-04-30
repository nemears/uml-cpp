#include "uml/initialNode.h"

using namespace UML;

ElementType InitialNode::getElementType() const {
    return ElementType::INITIAL_NODE;
}

bool InitialNode::isSubClassOf(ElementType eType) {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INITIAL_NODE;
    }

    return ret;
}