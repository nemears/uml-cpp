#include "uml/instanceSpecification.h"
#include "uml/slot.h"
#include "uml/classifier.h"
#include "uml/valueSpecification.h"
#include "uml/uml-stable.h"

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

void InstanceSpecification::reindexName(std::string oldName, std::string newName) {
    PackageableElement::reindexName(oldName, newName);
    m_classifiers.reindexName(oldName, newName);
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
    m_classifiers.m_signature = &InstanceSpecification::getClassifiers;
    m_classifiers.m_addFunctors.insert(new AddClassifierFunctor(this));
    m_classifiers.m_removeFunctors.insert(new RemoveClassifierFunctor(this));
    m_specification.subsets(*m_ownedElements);
    m_specification.m_signature = &InstanceSpecification::getSpecificationSingleton;
    m_slots.subsets(*m_ownedElements);
    m_slots.opposite(&Slot::getOwningInstanceSingleton);
    m_slots.m_signature = &InstanceSpecification::getSlots;
}

void InstanceSpecification::copy(const InstanceSpecification& rhs) {
    m_classifiers = rhs.m_classifiers;
    m_specification = rhs.m_specification;
    m_slots = rhs.m_slots;
}

InstanceSpecification::InstanceSpecification() : Element(ElementType::INSTANCE_SPECIFICATION) {
    init();
}

InstanceSpecification::InstanceSpecification(const InstanceSpecification& rhs) : Element(rhs, ElementType::INSTANCE_SPECIFICATION) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    PackageableElement::copy(rhs);
    DeploymentTarget::copy(rhs);
    DeployedArtifact::copy(rhs);
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