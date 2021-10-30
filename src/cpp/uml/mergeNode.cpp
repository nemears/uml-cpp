#include "uml/mergeNode.h"

using namespace UML;

MergeNode::MergeNode() : Element(ElementType::MERGE_NODE) {}

MergeNode::MergeNode(const MergeNode& rhs) : Element(rhs, ElementType::MERGE_NODE) {}

bool MergeNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MERGE_NODE;
    }

    return ret;
}