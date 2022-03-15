#include "uml/behavior.h"
#include "uml/behavioralFeature.h"
#include "uml/dataType.h"
#include "uml/operation.h"
#include "uml/parameter.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Behavior::referencingReleased(ID id) {
    Class::referencingReleased(id);
    m_specification.release(id);
}

void Behavior::reindexName(ID id, std::string newName) {
    Class::reindexName(id, newName);
    m_specification.reindexName(id, newName);
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

Behavior::Behavior() : Element(ElementType::BEHAVIOR) {
    init();
}

Behavior::~Behavior() {
    
}

Set<Parameter, Behavior>& Behavior::getOwnedParameters() {
    return m_ownedParameters;
}

BehavioralFeaturePtr Behavior::getSpecification() const {
    return m_specification.get();
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