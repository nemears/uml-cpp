#include "uml/initialNode.h"

using namespace UML;

InitialNode::InitialNode() : Element(ElementType::INITIAL_NODE) {}

InitialNode::InitialNode(const InitialNode& rhs) : Element(rhs, ElementType::INITIAL_NODE) {}

bool InitialNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INITIAL_NODE;
    }

    return ret;
}