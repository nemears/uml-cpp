#include "uml/uml-stable.h"

using namespace UML;

Singleton<ValueSpecification, InstanceSpecification>& InstanceSpecification::getSpecificationSingleton() {
    return m_specification;
}

InstanceSpecification::InstanceSpecification(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    DeployedArtifact(elementType, manager),
    DeploymentTarget(elementType, manager)
{
    m_specification.subsets(m_ownedElements);
    m_slots.subsets(m_ownedElements);
    m_slots.opposite(&Slot::getOwningInstanceSingleton);
}

IndexableSet<Classifier, InstanceSpecification>& InstanceSpecification::getClassifiers() {
    return m_classifiers;
}

ValueSpecificationPtr InstanceSpecification::getSpecification() const {
    return m_specification.get();
}

void InstanceSpecification::setSpecification(ValueSpecificationPtr specification) {
    m_specification.set(specification);
}

void InstanceSpecification::setSpecification(ValueSpecification& specification) {
    m_specification.set(specification);
}

void InstanceSpecification::setSpecification(ID id) {
    m_specification.set(id);
}

Set<Slot, InstanceSpecification>& InstanceSpecification::getSlots() {
    return m_slots;
}
