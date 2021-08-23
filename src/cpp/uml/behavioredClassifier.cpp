#include "uml/behavioredClassifier.h"
#include "uml/behavior.h"
#include "uml/universalFunctions.h"

using namespace UML;

void BehavioredClassifier::AddOwnedBehaviorFunctor::operator()(Behavior& el) const {
    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    el.setBehavioredClassifier(m_el);
}

void BehavioredClassifier::RemoveOwnedBehaviorFunctor::operator()(Behavior& el) const {
    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().remove(el);
    }

    el.setBehavioredClassifier(0);
}

void BehavioredClassifier::setManager(UmlManager* manager) {
    Classifier::setManager(manager);
    m_ownedBehaviors.m_manager = manager;
}

BehavioredClassifier::BehavioredClassifier() {
    m_ownedBehaviors.addProcedures.push_back(new AddOwnedBehaviorFunctor(this));
    m_ownedBehaviors.removeProcedures.push_back(new RemoveOwnedBehaviorFunctor(this));
    m_classifierBehaviorPtr = 0;
}

BehavioredClassifier::BehavioredClassifier(const BehavioredClassifier& classifier) {
    m_ownedBehaviors = classifier.m_ownedBehaviors;
    m_ownedBehaviors.addProcedures.clear();
    m_ownedBehaviors.removeProcedures.clear();
    m_ownedBehaviors.addProcedures.push_back(new AddOwnedBehaviorFunctor(this));
    m_ownedBehaviors.removeProcedures.push_back(new RemoveOwnedBehaviorFunctor(this));
}

BehavioredClassifier::~BehavioredClassifier() {

}

Sequence<Behavior>& BehavioredClassifier::getOwnedBehaviors() {
    return m_ownedBehaviors;
}

Behavior* BehavioredClassifier::getClassifierBehavior() {
    return universalGet<Behavior>(m_classifierBehaviorID, m_classifierBehaviorPtr, m_manager);
}

void BehavioredClassifier::setClassifierBehavior(Behavior* behavior) {
    if (!isSameOrNull(m_classifierBehaviorID, behavior)) {
        if (!m_classifierBehaviorPtr) {
            m_classifierBehaviorPtr = &m_manager->get<Behavior>(m_classifierBehaviorID);
        }

        if (m_ownedBehaviors.count(m_classifierBehaviorID)) {
            m_ownedBehaviors.remove(*m_classifierBehaviorPtr);
        }

        m_classifierBehaviorID = ID::nullID();
        m_classifierBehaviorPtr = 0;
    }

    if (behavior) {
        m_classifierBehaviorID = behavior->getID();
    }

    if (!m_manager) {
        m_classifierBehaviorPtr = behavior;
    }

    if (behavior) {
        if (!m_ownedBehaviors.count(behavior->getID())) {
            m_ownedBehaviors.add(*behavior);
        }
    }
}

ElementType BehavioredClassifier::getElementType() const {
    return ElementType::BEHAVIORED_CLASSIFIER;
}

bool BehavioredClassifier::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::BEHAVIORED_CLASSIFIER;
    }

    return ret;
}