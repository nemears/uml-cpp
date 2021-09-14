#include "uml/behavioredClassifier.h"
#include "uml/behavior.h"

using namespace UML;

void BehavioredClassifier::RemoveClassifierBehaviorProcedure::operator()(Behavior* el) const {
    if (m_me->getOwnedBehaviors().count(el->getID())) {
        m_me->getOwnedBehaviors().remove(*el);
    }
}

void BehavioredClassifier::AddClassifierBehaviorProcedure::operator()(Behavior* el) const {
    if (!m_me->getOwnedBehaviors().count(el->getID())) {
        m_me->getOwnedBehaviors().add(*el);
    }
}

void BehavioredClassifier::AddOwnedBehaviorFunctor::operator()(Behavior& el) const {
    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    el.setBehavioredClassifier(m_el);
    updateCopiedSequenceAddedTo(el, &BehavioredClassifier::getOwnedBehaviors);
}

void BehavioredClassifier::RemoveOwnedBehaviorFunctor::operator()(Behavior& el) const {
    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().remove(el);
    }

    el.setBehavioredClassifier(0);
    updateCopiedSequenceRemovedFrom(el, &BehavioredClassifier::getOwnedBehaviors);
}

void BehavioredClassifier::setManager(UmlManager* manager) {
    Classifier::setManager(manager);
    m_ownedBehaviors.m_manager = manager;
}

void BehavioredClassifier::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    m_ownedBehaviors.elementReleased(id, &BehavioredClassifier::getOwnedBehaviors);
    if (m_classifierBehavior.id() == id) {
        m_classifierBehavior.release();
    }
}

void BehavioredClassifier::referenceReindexed(ID oldID, ID newID) {
    Classifier::referenceReindexed(oldID, newID);
    if (m_ownedBehaviors.count(oldID)) {
        m_ownedBehaviors.reindex(oldID, newID, &BehavioredClassifier::getOwnedBehaviors);
    }
    if (m_classifierBehavior.id() == oldID) {
        m_classifierBehavior.reindex(oldID, newID);
    }
}

BehavioredClassifier::BehavioredClassifier() {
    m_ownedBehaviors.addProcedures.push_back(new AddOwnedBehaviorFunctor(this));
    m_ownedBehaviors.removeProcedures.push_back(new RemoveOwnedBehaviorFunctor(this));
    m_classifierBehavior.m_signature = &BehavioredClassifier::m_classifierBehavior;
    m_classifierBehavior.m_removeProcedures.push_back(new RemoveClassifierBehaviorProcedure(this));
    m_classifierBehavior.m_addProcedures.push_back(new AddClassifierBehaviorProcedure(this));
}

BehavioredClassifier::BehavioredClassifier(const BehavioredClassifier& classifier) {
    m_ownedBehaviors = classifier.m_ownedBehaviors;
    m_ownedBehaviors.m_el = this;
    m_ownedBehaviors.addProcedures.clear();
    m_ownedBehaviors.removeProcedures.clear();
    m_ownedBehaviors.addProcedures.push_back(new AddOwnedBehaviorFunctor(this));
    m_ownedBehaviors.removeProcedures.push_back(new RemoveOwnedBehaviorFunctor(this));
    m_classifierBehavior = classifier.m_classifierBehavior;
    m_classifierBehavior.m_me = this;
    m_classifierBehavior.m_removeProcedures.clear();
    m_classifierBehavior.m_addProcedures.clear();
    m_classifierBehavior.m_removeProcedures.push_back(new RemoveClassifierBehaviorProcedure(this));
    m_classifierBehavior.m_addProcedures.push_back(new AddClassifierBehaviorProcedure(this));
}

BehavioredClassifier::~BehavioredClassifier() {

}

Sequence<Behavior>& BehavioredClassifier::getOwnedBehaviors() {
    return m_ownedBehaviors;
}

Behavior* BehavioredClassifier::getClassifierBehavior() {
    return m_classifierBehavior.get();
}

Behavior& BehavioredClassifier::getClassifierBehaviorRef() {
    return m_classifierBehavior.getRef();
}

bool BehavioredClassifier::hasClassifierBehavior() const {
    return m_classifierBehavior.has();
}

void BehavioredClassifier::setClassifierBehavior(Behavior* behavior) {
    m_classifierBehavior.set(behavior);
}

void BehavioredClassifier::setClassifierBehavior(Behavior& behavior) {
    m_classifierBehavior.set(behavior);
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