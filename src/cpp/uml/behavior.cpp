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

void Behavior::referenceReindexed(ID newID) {
    Class::referenceReindexed(newID);
    m_specification.reindex(newID);
}

void Behavior::referenceErased(ID id) {
    Class::referenceErased(id);
    m_specification.eraseElement(id);
}

TypedSet<BehavioralFeature, Behavior>& Behavior::getSpecificationSingleton() {
    return m_specification;
}

void Behavior::init() {
    m_ownedParameters.subsets(m_ownedMembers);
    m_specification.opposite(&BehavioralFeature::getMethods);
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