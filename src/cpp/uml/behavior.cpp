#include "uml/behavior.h"
#include "uml/behavioralFeature.h"
#include "uml/operation.h"
#include "uml/parameter.h"
#include "uml/uml-stable.h"

using namespace UML;

void Behavior::referencingReleased(ID id) {
    Class::referencingReleased(id);
    m_specification.release(id);
}

void Behavior::referenceReindexed(ID oldID, ID newID) {
    Class::referenceReindexed(oldID, newID);
    m_specification.reindex(oldID, newID);
}

void Behavior::referenceErased(ID id) {
    Class::referenceErased(id);
    m_specification.eraseElement(id);
}

Set<BehavioralFeature, Behavior>& Behavior::getSpecificationSingleton() {
    return m_specification;
}

void Behavior::init() {
    m_ownedParameters.subsets(m_ownedMembers);
    m_ownedParameters.m_signature = &Behavior::getOwnedParameters;
    m_specification.opposite(&BehavioralFeature::getMethods);
    m_specification.m_signature = &Behavior::getSpecificationSingleton;
}

void Behavior::copy(const Behavior& rhs) {
    m_ownedParameters = rhs.m_ownedParameters;
    m_specification = rhs.m_specification;
}

Behavior::Behavior() : Element(ElementType::BEHAVIOR) {
    init();
}

Behavior::Behavior(const Behavior& rhs) : Element(ElementType::BEHAVIOR) {
    // abstract
}

Behavior::~Behavior() {
    
}

Set<Parameter, Behavior>& Behavior::getOwnedParameters() {
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

void Behavior::setSpecification(ID id) {
    m_specification.set(id);
}

bool Behavior::isSubClassOf(ElementType eType) const {
    bool ret = Class::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::BEHAVIOR;
    }
    
    return ret;
}