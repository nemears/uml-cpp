#include "uml/behavior.h"
#include "uml/behavioralFeature.h"
#include "uml/operation.h"
#include "uml/parameter.h"

using namespace UML;

void Behavior::RemoveSpecificationProcedure::operator()(BehavioralFeature* el) const {
    if (el->getMethods().count(m_me->getID())) {
        el->getMethods().remove(*m_me);
    }
}

void Behavior::AddSpecificationProcedure::operator()(BehavioralFeature* el) const {
    if (!el->getMethods().count(m_me->getID())) {
        el->getMethods().add(*m_me);
    }
}

void Behavior::RemoveBehavioredClassifierProcedure::operator()(BehavioredClassifier* el) const {
    if (el->getOwnedBehaviors().count(m_me->getID())) {
        el->getOwnedBehaviors().remove(*m_me);
    }
}

void Behavior::AddBehavioredClassifierProcedure::operator()(BehavioredClassifier* el) const {
    if (!el->getOwnedBehaviors().count(m_me->getID())) {
        el->getOwnedBehaviors().add(*m_me);
    }
    if (m_me->getNamespaceID() != el->getID()) {
        m_me->setNamespace(el);
    }
}

void Behavior::AddBehavioredClassifierProcedure::operator()(ID id) const {
    if (m_me->getNamespaceID() != id) {
        m_me->m_namespace.setByID(id);
    }
}

void Behavior::AddOwnedParameterFunctor::operator()(Parameter& el) const {
    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().internalAdd(el);
    }

    if (el.getBehaviorID() != m_el->getID()) {
        el.setBehavior(m_el);
    }
    updateCopiedSequenceAddedTo(el, &Behavior::getOwnedParameters);
}

void Behavior::RemoveOwnedParameterFunctor::operator()(Parameter& el) const {
    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().internalRemove(el);
    }
    if (el.getBehaviorID() == m_el->getID()) {
        el.setBehavior(0);
    }
    updateCopiedSequenceRemovedFrom(el, &Behavior::getOwnedParameters);
}

void Behavior::referencingReleased(ID id) {
    Class::referencingReleased(id);
    m_ownedParameters.elementReleased(id, &Behavior::getOwnedParameters);
    m_specification.release(id);
    m_behavioredClassifier.release(id);
}

void Behavior::referenceReindexed(ID oldID, ID newID) {
    Class::referenceReindexed(oldID, newID);
    m_ownedParameters.reindex(oldID, newID, &Behavior::getOwnedParameters);
    m_specification.reindex(oldID, newID);
    m_behavioredClassifier.reindex(oldID, newID);
}

void Behavior::restoreReferences() {
    Class::restoreReferences();
    m_ownedParameters.restoreReferences();
    m_specification.restoreReference();
    m_behavioredClassifier.restoreReference();
}

void Behavior::referenceErased(ID id) {
    Class::referenceErased(id);
    m_ownedParameters.elementErased(id);
    m_specification.elementErased(id);
    m_behavioredClassifier.elementErased(id);
}

Behavior::Behavior() : Element(ElementType::BEHAVIOR) {
    m_ownedParameters.addProcedures.push_back(new AddOwnedParameterFunctor(this));
    m_ownedParameters.removeProcedures.push_back(new RemoveOwnedParameterFunctor(this));
    m_specification.m_signature = &Behavior::m_specification;
    m_specification.m_removeProcedures.push_back(new RemoveSpecificationProcedure(this));
    m_specification.m_addProcedures.push_back(new AddSpecificationProcedure(this));
    m_behavioredClassifier.m_signature = &Behavior::m_behavioredClassifier;
    m_behavioredClassifier.m_removeProcedures.push_back(new RemoveBehavioredClassifierProcedure(this));
    m_behavioredClassifier.m_addProcedures.push_back(new AddBehavioredClassifierProcedure(this));
}

Behavior::Behavior(const Behavior& rhs) : 
Class(rhs), 
Classifier(rhs), 
PackageableElement(rhs), 
NamedElement(rhs),
Element(rhs, ElementType::BEHAVIOR) {
    m_ownedParameters = rhs.m_ownedParameters;
    m_ownedParameters.m_el = this;
    m_ownedParameters.addProcedures.clear();
    m_ownedParameters.removeProcedures.clear();
    m_ownedParameters.addProcedures.push_back(new AddOwnedParameterFunctor(this));
    m_ownedParameters.removeProcedures.push_back(new RemoveOwnedParameterFunctor(this));
    m_specification = rhs.m_specification;
    m_specification.m_me = this;
    m_specification.m_removeProcedures.clear();
    m_specification.m_addProcedures.clear();
    m_specification.m_removeProcedures.push_back(new RemoveSpecificationProcedure(this));
    m_specification.m_addProcedures.push_back(new AddSpecificationProcedure(this));
    m_behavioredClassifier = rhs.m_behavioredClassifier;
    m_behavioredClassifier.m_me = this;
    m_behavioredClassifier.m_removeProcedures.clear();
    m_behavioredClassifier.m_addProcedures.clear();
    m_behavioredClassifier.m_removeProcedures.push_back(new RemoveBehavioredClassifierProcedure(this));
    m_behavioredClassifier.m_addProcedures.push_back(new AddBehavioredClassifierProcedure(this));
}

Behavior::~Behavior() {
    
}

Sequence<Parameter>& Behavior::getOwnedParameters() {
    return m_ownedParameters;
}

BehavioralFeature* Behavior::getSpecification() {
    return m_specification.get();
}

BehavioralFeature& Behavior::getSpecificationRef() {
    return m_specification.getRef();
}

ID Behavior::getSpecificationID() const {
    return m_specification.id();
}

bool Behavior::hasSpecification() const {
    return m_specification.has();
}

void Behavior::setSpecification(BehavioralFeature* specification) {
    m_specification.set(specification);
}

void Behavior::setSpecification(BehavioralFeature& specification) {
    m_specification.set(specification);
}

BehavioredClassifier* Behavior::getBehavioredClassifier() {
    return m_behavioredClassifier.get();
}

BehavioredClassifier& Behavior::getBehavioredClassifierRef() {
    return m_behavioredClassifier.getRef();
}

ID Behavior::getBehavioredClassifierID() const {
    return m_behavioredClassifier.id();
}

bool Behavior::hasBehavioredClassifier() const {
    return m_behavioredClassifier.has();
}

void Behavior::setBehavioredClassifier(BehavioredClassifier* classifier) {
    m_behavioredClassifier.set(classifier);
}

void Behavior::setBehavioredClassifier(BehavioredClassifier& classifier) {
    m_behavioredClassifier.set(classifier);
}

bool Behavior::isSubClassOf(ElementType eType) const {
    bool ret = Class::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::BEHAVIOR;
    }
    
    return ret;
}