#include "uml/mergeNode.h"

using namespace UML;

ElementType MergeNode::getElementType() const {
    return ElementType::MERGE_NODE;
}

bool MergeNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MERGE_NODE;
    }

    return ret;
}