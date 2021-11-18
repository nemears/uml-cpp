#include "uml/instanceSpecification.h"
#include "uml/slot.h"
#include "uml/classifier.h"
#include "uml/valueSpecification.h"
#include "uml/uml-stable.h"

using namespace UML;

void InstanceSpecification::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    // DeployedArtifact::referenceReindexed(oldID, newID);
    m_classifiers.reindex(oldID, newID);
    m_slots.reindex(oldID, newID);
    m_specification.reindex(oldID, newID);
}

void InstanceSpecification::referencingReleased(ID id) {
    PackageableElement::referencingReleased(id);
    // DeployedArtifact::referencingReleased(id);
    m_classifiers.release(id);
    m_specification.release(id);
    m_slots.release(id);
}

void InstanceSpecification::restoreReferences() {
    // PackageableElement::restoreReferences();
    // m_classifier.restoreReference();
    // m_specification.restoreReference();
    // m_slots.restoreReferences();
}

void InstanceSpecification::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    m_classifiers.eraseElement(id);
    m_specification.eraseElement(id);
    m_slots.eraseElement(id);
}

void InstanceSpecification::init() {

}

void InstanceSpecification::copy(const InstanceSpecification& rhs) {

}

InstanceSpecification::InstanceSpecification() : Element(ElementType::INSTANCE_SPECIFICATION) {
    init();
}

InstanceSpecification::InstanceSpecification(const InstanceSpecification& rhs) : Element(rhs, ElementType::INSTANCE_SPECIFICATION) {
    init();
    NamedElement::copy(rhs);
    PackageableElement::copy(rhs);
    copy(rhs);
}

InstanceSpecification::~InstanceSpecification() {
    
}

Set<Classifier, InstanceSpecification>& InstanceSpecification::getClassifiers() {
    return m_classifiers;
}

ValueSpecification* InstanceSpecification::getSpecification() {
    return m_specification.get();
}

ValueSpecification& InstanceSpecification::getSpecificationRef() {
    return m_specification.getRef();
}

ID InstanceSpecification::getSpecificationID() const {
    return m_specification.id();
}

bool InstanceSpecification::hasSpecification() const {
    return m_specification.has();
}

void InstanceSpecification::setSpecification(ValueSpecification* specification) {
    m_specification.set(specification);
}

void InstanceSpecification::setSpecification(ValueSpecification& specification) {
    m_specification.set(specification);
}

Set<Slot, InstanceSpecification>& InstanceSpecification::getSlots() {
    return m_slots;
}

bool InstanceSpecification::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    // if (!ret) {
    //     ret = DeploymentTarget::isSubClassOf(eType);
    // }

    // if (!ret) {
    //     ret = DeployedArtifact::isSubClassOf(eType);
    // }

    if (!ret) {
        ret = eType == ElementType::INSTANCE_SPECIFICATION;
    }

    return ret;
}