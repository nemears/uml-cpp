#include "uml/uml-stable.h"

using namespace UML;

void Behavior::referenceErased(ID id) {
    Class::referenceErased(id);
    eraseFromSet(id, m_specification);
}

Singleton<BehavioralFeature, Behavior>& Behavior::getSpecificationSingleton() {
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

bool Behavior::is(ElementType eType) const {
    bool ret = Class::is(eType);

    if (!ret) {
        ret = eType == ElementType::BEHAVIOR;
    }
    
    return ret;
}
