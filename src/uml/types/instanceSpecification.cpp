#include "uml/set/singleton.h"
#include "uml/uml-stable.h"

using namespace UML;

void InstanceSpecification::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    eraseFromSet(id, m_classifiers);
}

Singleton<ValueSpecification, InstanceSpecification>& InstanceSpecification::getSpecificationSingleton() {
    return m_specification;
}

InstanceSpecification::InstanceSpecification() : Element(ElementType::INSTANCE_SPECIFICATION) {
    m_specification.subsets(*m_ownedElements);
    m_slots.subsets(*m_ownedElements);
    m_slots.opposite(&Slot::getOwningInstanceSingleton);
}

InstanceSpecification::~InstanceSpecification() {
    
}

Set<Classifier, InstanceSpecification>& InstanceSpecification::getClassifiers() {
    return m_classifiers;
}

ValueSpecificationPtr InstanceSpecification::getSpecification() const {
    return m_specification.get();
}

void InstanceSpecification::setSpecification(ValueSpecification* specification) {
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

bool InstanceSpecification::is(ElementType eType) const {
    bool ret = PackageableElement::is(eType);

    if (!ret) {
        ret = DeploymentTarget::is(eType);
    }

    if (!ret) {
        ret = DeployedArtifact::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::INSTANCE_SPECIFICATION;
    }

    return ret;
}
