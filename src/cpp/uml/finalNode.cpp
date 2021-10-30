#include "uml/finalNode.h"

using namespace UML;

FinalNode::FinalNode() : Element(ElementType::FINAL_NODE) {}

FinalNode::FinalNode(const FinalNode& rhs) : Element(rhs, ElementType::FINAL_NODE) {}

bool FinalNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::FINAL_NODE;
    }

    return ret;
}