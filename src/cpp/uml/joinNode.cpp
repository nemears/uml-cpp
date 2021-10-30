#include "uml/joinNode.h"

using namespace UML;

JoinNode::JoinNode() : Element(ElementType::JOIN_NODE) {}

JoinNode::JoinNode(const JoinNode& rhs) : Element(rhs, ElementType::JOIN_NODE) {}

bool JoinNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::JOIN_NODE;
    }

    return ret;
}