#include "uml/managers/abstractManager.h"
#include "uml/types/namedElement.h"
#include "uml/types/packageableElement.h"
#include "uml/types/parameterableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

Singleton<BehavioralFeature, Behavior>& Behavior::getSpecificationSingleton() {
    return m_specification;
}

Behavior::Behavior(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager),
    Class(elementType, manager)
{
    m_ownedParameters.subsets(m_ownedMembers);
    m_specification.opposite(&BehavioralFeature::getMethods);
}

Set<Parameter, Behavior>& Behavior::getOwnedParameters() {
    return m_ownedParameters;
}

BehavioralFeaturePtr Behavior::getSpecification() const {
    return m_specification.get();
}

void Behavior::setSpecification(BehavioralFeaturePtr specification) {
    m_specification.set(specification);
}

void Behavior::setSpecification(BehavioralFeature& specification) {
    m_specification.set(specification);
}

void Behavior::setSpecification(ID id) {
    m_specification.set(id);
}
