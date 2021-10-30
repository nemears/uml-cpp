#include "uml/forkNode.h"

using namespace UML;

ForkNode::ForkNode() : Element(ElementType::FORK_NODE) {}

ForkNode::ForkNode(const ForkNode& rhs) : Element(rhs, ElementType::FORK_NODE) {}

bool ForkNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::FORK_NODE;
    }

    return ret;
}