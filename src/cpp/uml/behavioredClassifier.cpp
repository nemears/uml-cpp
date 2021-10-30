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

void BehavioredClassifier::AddClassifierBehaviorProcedure::operator()(ID id) const {
    if (!m_me->getOwnedBehaviors().count(id)) {
        m_me->getOwnedBehaviors().addByID(id);
    }
}

void BehavioredClassifier::AddOwnedBehaviorFunctor::operator()(Behavior& el) const {
    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    el.setBehavioredClassifier(m_el);
    updateCopiedSequenceAddedTo(el, &BehavioredClassifier::getOwnedBehaviors);
}

void BehavioredClassifier::AddOwnedBehaviorFunctor::operator()(ID id) const {
    if (!m_el->getOwnedMembers().count(id)) {
        m_el->getOwnedMembers().addByID(id);
    }
}

void BehavioredClassifier::RemoveOwnedBehaviorFunctor::operator()(Behavior& el) const {
    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().remove(el);
    }

    el.setBehavioredClassifier(0);
    updateCopiedSequenceRemovedFrom(el, &BehavioredClassifier::getOwnedBehaviors);
}

void BehavioredClassifier::referencingReleased(ID id) {
    m_ownedBehaviors.elementReleased(id, &BehavioredClassifier::getOwnedBehaviors);
    m_classifierBehavior.release(id);
}

void BehavioredClassifier::referenceReindexed(ID oldID, ID newID) {
    m_ownedBehaviors.reindex(oldID, newID, &BehavioredClassifier::getOwnedBehaviors);
    m_classifierBehavior.reindex(oldID, newID);
}

void BehavioredClassifier::restoreReferences() {
    m_ownedBehaviors.restoreReferences();
    m_classifierBehavior.restoreReference();
}

void BehavioredClassifier::referenceErased(ID id) {
    m_ownedBehaviors.elementErased(id);
    m_classifierBehavior.elementErased(id);
}

BehavioredClassifier::BehavioredClassifier() : Element(ElementType::BEHAVIORED_CLASSIFIER) {
    m_ownedBehaviors.addProcedures.push_back(new AddOwnedBehaviorFunctor(this));
    m_ownedBehaviors.removeProcedures.push_back(new RemoveOwnedBehaviorFunctor(this));
    m_classifierBehavior.m_signature = &BehavioredClassifier::m_classifierBehavior;
    m_classifierBehavior.m_removeProcedures.push_back(new RemoveClassifierBehaviorProcedure(this));
    m_classifierBehavior.m_addProcedures.push_back(new AddClassifierBehaviorProcedure(this));
}

BehavioredClassifier::BehavioredClassifier(const BehavioredClassifier& classifier) : Element(classifier, ElementType::BEHAVIORED_CLASSIFIER) {
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

ID BehavioredClassifier::getClassifierBehaviorID() const {
    return m_classifierBehavior.id();
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

bool BehavioredClassifier::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::BEHAVIORED_CLASSIFIER;
    }

    return ret;
}