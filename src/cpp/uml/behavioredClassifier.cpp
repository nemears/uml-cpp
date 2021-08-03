#include "uml/behavioredClassifier.h"
#include "uml/behavior.h"

using namespace UML;

void BehavioredClassifier::AddOwnedBehaviorFunctor::operator()(Element& el) const {
    if (!m_el->as<BehavioredClassifier>().getOwnedMembers().count(el.getID())) {
        m_el->as<BehavioredClassifier>().getOwnedMembers().add(el.as<Behavior>());
    }
}

void BehavioredClassifier::RemoveOwnedBehaviorFunctor::operator()(Element& el) const {
    if (m_el->as<BehavioredClassifier>().getOwnedMembers().count(el.getID())) {
        m_el->as<BehavioredClassifier>().getOwnedMembers().remove(el.as<Behavior>());
    }
}

void BehavioredClassifier::setManager(UmlManager* manager) {
    Classifier::setManager(manager);
    m_ownedBehaviors.m_manager = manager;
}

BehavioredClassifier::BehavioredClassifier() {
    m_ownedBehaviors.addProcedures.push_back(new AddOwnedBehaviorFunctor(this));
    m_ownedBehaviors.removeProcedures.push_back(new RemoveOwnedBehaviorFunctor(this));
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