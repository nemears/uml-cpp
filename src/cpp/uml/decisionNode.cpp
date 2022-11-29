#include "uml/decisionNode.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interruptibleActivityRegion.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<ObjectFlow, DecisionNode>& DecisionNode::getDecisionInputFlowSingleton() {
    return m_decisionInputFlow;
}

TypedSet<Behavior, DecisionNode>& DecisionNode::getDecisionInputSingleton() {
    return m_decisionInput;
}

void DecisionNode::referenceReindexed(ID newID) {
    ControlNode::referenceReindexed(newID);
    m_decisionInputFlow.reindex(newID);
    m_decisionInput.reindex(newID);
}

void DecisionNode::referenceErased(ID id) {
    ControlNode::referenceErased(id);
    m_decisionInputFlow.eraseElement(id);
    m_decisionInput.eraseElement(id);
}

void DecisionNode::init() {}

DecisionNode::DecisionNode() : Element(ElementType::DECISION_NODE) {
    init();
}

DecisionNode::~DecisionNode() {

}

ObjectFlowPtr DecisionNode::getDecisionInputFlow() const {
    return m_decisionInputFlow.get();
}

void DecisionNode::setDecisionInputFlow(ObjectFlow* inputFlow) {
    m_decisionInputFlow.set(inputFlow);
}

void DecisionNode::setDecisionInputFlow(ObjectFlow& flow) {
    m_decisionInputFlow.set(flow);
}

void DecisionNode::setDecisionInputFlow(ID id) {
    m_decisionInputFlow.set(id);
}

BehaviorPtr DecisionNode::getDecisionInput() const {
    return m_decisionInput.get();
}

void DecisionNode::setDecisionInput(Behavior* input) {
    m_decisionInput.set(input);
}

void DecisionNode::setDecisionInput(Behavior& input) {
    m_decisionInput.set(input);
}

void DecisionNode::setDecisionInput(ID id) {
    m_decisionInput.set(id);
}

bool DecisionNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DECISION_NODE;
    }

    return ret;
}