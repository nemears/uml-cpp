#include "uml/behavioredClassifier.h"
#include "uml/behavior.h"
#include "uml/uml-stable.h"

using namespace UML;

Set<Behavior, BehavioredClassifier>& BehavioredClassifier::getClassifierBehaviorSingleton() {
    return m_classifierBehavior;
}

void BehavioredClassifier::init() {
    m_ownedBehaviors.subsets(m_ownedMembers);
    m_ownedBehaviors.m_signature = &BehavioredClassifier::getOwnedBehaviors;
    m_classifierBehavior.subsets(m_ownedBehaviors);
    m_classifierBehavior.m_signature = &BehavioredClassifier::getClassifierBehaviorSingleton;
    m_interfaceRealizations.subsets(*m_ownedElements);
    m_interfaceRealizations.opposite(&InterfaceRealization::getImplementingClassifierSingleton);
    m_interfaceRealizations.m_signature = &BehavioredClassifier::getInterfaceRealizations;
}

void BehavioredClassifier::copy(const BehavioredClassifier& rhs) {
    m_ownedBehaviors = rhs.m_ownedBehaviors;
    m_classifierBehavior = rhs.m_classifierBehavior;
    m_interfaceRealizations = rhs.m_interfaceRealizations;
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

Set<InterfaceRealization, BehavioredClassifier>& BehavioredClassifier::getInterfaceRealizations() {
    return m_interfaceRealizations;
}

bool BehavioredClassifier::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::BEHAVIORED_CLASSIFIER;
    }

    return ret;
}