#include "uml/decisionNode.h"

using namespace UML;

ObjectFlow* DecisionNode::getDecisionInputFlow() {
    return decisionInputFlow;
}

void DecisionNode::setDecisionInputFlow(ObjectFlow* inputFlow) {
    this->decisionInputFlow = inputFlow;
}

ElementType DecisionNode::getElementType() {
    return ElementType::DECISION_NODE;
}