#include "uml/finalNode.h"

using namespace UML;

ElementType FinalNode::getElementType() const {
    return ElementType::FINAL_NODE;
}

bool FinalNode::isSubClassOf(ElementType eType) {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::FINAL_NODE;
    }

    return ret;
}