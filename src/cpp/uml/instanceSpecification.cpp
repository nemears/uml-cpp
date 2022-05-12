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

using namespace UML;

void InstanceSpecification::AddClassifierFunctor::operator()(Element& el) const {
    el.setReference(&m_el);
}

void InstanceSpecification::RemoveClassifierFunctor::operator()(Element& el) const {
    el.removeReference(el.getID());
}

void InstanceSpecification::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    m_classifiers.reindex(oldID, newID);
}

void InstanceSpecification::reindexName(ID id, std::string newName) {
    PackageableElement::reindexName(id, newName);
    m_classifiers.reindexName(id, newName);
}

void InstanceSpecification::referencingReleased(ID id) {
    PackageableElement::referencingReleased(id);
    m_classifiers.release(id);
}

void InstanceSpecification::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    m_classifiers.eraseElement(id);
}

Set<ValueSpecification, InstanceSpecification>& InstanceSpecification::getSpecificationSingleton() {
    return m_specification;
}

void InstanceSpecification::init() {
    m_classifiers.m_addFunctors.insert(new AddClassifierFunctor(this));
    m_classifiers.m_removeFunctors.insert(new RemoveClassifierFunctor(this));
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