#include "uml/behavioredClassifier.h"
#include "uml/behavior.h"
#include "uml/uml-stable.h"

using namespace UML;

void BehavioredClassifier::referencingReleased(ID id) {
    m_ownedBehaviors.release(id);
    m_classifierBehavior.release(id);
}

void BehavioredClassifier::referenceReindexed(ID oldID, ID newID) {
    m_ownedBehaviors.reindex(oldID, newID);
    m_classifierBehavior.reindex(oldID, newID);
}

void BehavioredClassifier::referenceErased(ID id) {
    m_ownedBehaviors.eraseElement(id);
    m_classifierBehavior.eraseElement(id);
}

Set<Behavior, BehavioredClassifier>& BehavioredClassifier::getClassifierBehaviorSingleton() {
    return m_classifierBehavior;
}

void BehavioredClassifier::init() {
    m_ownedBehaviors.subsets(m_ownedMembers);
    m_ownedBehaviors.m_signature = &BehavioredClassifier::getOwnedBehaviors;
    m_classifierBehavior.subsets(m_ownedBehaviors);
    m_classifierBehavior.m_signature = &BehavioredClassifier::getClassifierBehaviorSingleton;
}

void BehavioredClassifier::copy(const BehavioredClassifier& rhs) {
    m_ownedBehaviors = rhs.m_ownedBehaviors;
    m_classifierBehavior = rhs.m_classifierBehavior;
}

BehavioredClassifier::BehavioredClassifier() : Element(ElementType::BEHAVIORED_CLASSIFIER) {
    init();
}

BehavioredClassifier::BehavioredClassifier(const BehavioredClassifier& classifier) : Element(classifier, ElementType::BEHAVIORED_CLASSIFIER) {
    // abstract
}

BehavioredClassifier::~BehavioredClassifier() {

}

Set<Behavior, BehavioredClassifier>& BehavioredClassifier::getOwnedBehaviors() {
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

void BehavioredClassifier::setClassifierBehavior(ID id) {
    m_classifierBehavior.set(id);
}

bool BehavioredClassifier::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::BEHAVIORED_CLASSIFIER;
    }

    return ret;
}