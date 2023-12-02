#include "uml/types/behavior.h"
#include "uml/types/behavioralFeature.h"
#include "uml/types/dataType.h"
#include "uml/types/operation.h"
#include "uml/types/parameter.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Behavior::referenceErased(ID id) {
    Class::referenceErased(id);
    m_specification.eraseElement(id);
}

TypedSet<BehavioralFeature, Behavior>& Behavior::getSpecificationSingleton() {
    return m_specification;
}

Behavior::Behavior() : Element(ElementType::BEHAVIOR) {
    m_ownedParameters.subsets(m_ownedMembers);
    m_specification.opposite(&BehavioralFeature::getMethods);
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