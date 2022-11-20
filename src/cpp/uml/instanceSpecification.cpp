#include "uml/instanceSpecification.h"
#include "uml/slot.h"
#include "uml/behavior.h"
#include "uml/valueSpecification.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/property.h"
#include "uml/package.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

#include <iostream>

using namespace UML;

void InstanceSpecification::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    m_classifiers.reindex(newID);
}

void InstanceSpecification::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    m_classifiers.eraseElement(id);
}

TypedSet<ValueSpecification, InstanceSpecification>& InstanceSpecification::getSpecificationSingleton() {
    return m_specification;
}

void InstanceSpecification::init() {
    m_specification.subsets(*m_ownedElements);
    m_slots.subsets(*m_ownedElements);
    m_slots.opposite(&Slot::getOwningInstanceSingleton);
}

InstanceSpecification::InstanceSpecification() : Element(ElementType::INSTANCE_SPECIFICATION) {
    init();
}

InstanceSpecification::~InstanceSpecification() {
    mountAndRelease();
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