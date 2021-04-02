#include "uml/mergeNode.h"

using namespace UML;

ElementType MergeNode::getElementType() {
    return ElementType::MERGE_NODE;
}

bool MergeNode::isSubClassOf(ElementType eType) {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MERGE_NODE;
    }

    return ret;
}