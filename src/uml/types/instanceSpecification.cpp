#include "uml/types/instanceSpecification.h"
#include "uml/types/slot.h"
#include "uml/types/behavior.h"
#include "uml/types/valueSpecification.h"
#include "uml/types/generalization.h"
#include "uml/types/dataType.h"
#include "uml/types/property.h"
#include "uml/types/package.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

#include <iostream>

using namespace UML;

void InstanceSpecification::referenceReindexed(ID newID) {
    PackageableElement::referenceReindexed(newID);
    m_classifiers.reindex(newID);
}

void InstanceSpecification::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    m_classifiers.eraseElement(id);
}

TypedSet<ValueSpecification, InstanceSpecification>& InstanceSpecification::getSpecificationSingleton() {
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

bool InstanceSpecification::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = DeploymentTarget::isSubClassOf(eType);
    }

    if (!ret) {
        ret = DeployedArtifact::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::INSTANCE_SPECIFICATION;
    }

    return ret;
}