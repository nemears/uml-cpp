#include "uml/decisionNode.h"

using namespace UML;

DecisionNode::DecisionNode() : Element(ElementType::DECISION_NODE) {
    decisionInputFlow = 0;
}

DecisionNode::DecisionNode(const DecisionNode& rhs) :
ActivityNode(rhs),
RedefinableElement(rhs),
NamedElement(rhs),
Element(rhs, ElementType::DECISION_NODE) {}

ObjectFlow* DecisionNode::getDecisionInputFlow() {
    return decisionInputFlow;
}

void DecisionNode::setDecisionInputFlow(ObjectFlow* inputFlow) {
    this->decisionInputFlow = inputFlow;
}

bool DecisionNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DECISION_NODE;
    }

    return ret;
}