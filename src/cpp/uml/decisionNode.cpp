#include "uml/decisionNode.h"

using namespace UML;

ObjectFlow* DecisionNode::getDecisionInputFlow() {
    return decisionInputFlow;
}

void DecisionNode::setDecisionInputFlow(ObjectFlow* inputFlow) {
    this->decisionInputFlow = inputFlow;
}

ElementType DecisionNode::getElementType() const {
    return ElementType::DECISION_NODE;
}

bool DecisionNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DECISION_NODE;
    }

    return ret;
}