#include "uml/slot.h"
#include "uml/instanceSpecification.h"
#include "uml/structuralFeature.h"
#include "uml/valueSpecification.h"
#include "uml/uml-stable.h"

using namespace UML;

void Slot::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_definingFeature.release(id);
    m_values.release(id);
    m_owningInstance.release(id);
}

void Slot::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_definingFeature.reindex(oldID, newID);
    m_values.reindex(oldID, newID);
    m_owningInstance.reindex(oldID, newID);
}

void Slot::restoreReferences() {
    Element::restoreReferences();
    // m_definingFeature.restoreReference();
    // m_values.restoreReferences();
    // m_owningInstance.restoreReference();
}

void Slot::referenceErased(ID id) {
    Element::referenceErased(id);
    m_definingFeature.eraseElement(id);
    m_values.eraseElement(id);
    m_owningInstance.eraseElement(id);
}

Set<StructuralFeature, Slot>& Slot::getDefiningFeatureSingleton() {
    return m_definingFeature;
}

Set<InstanceSpecification, Slot>& Slot::getOwningInstanceSingleton() {
    return m_owningInstance;
}

void Slot::init() {

}

void Slot::copy(const Slot& rhs) {

}

Slot::Slot() : Element(ElementType::SLOT) {
    init();
}

Slot::Slot(const Slot& rhs) : Element(rhs, ElementType::SLOT) {
    init();
    copy(rhs);
}

Slot::~Slot() {

}

Set<ValueSpecification, Slot>& Slot::getValues() {
    return m_values;
}

StructuralFeature* Slot::getDefiningFeature() {
    return m_definingFeature.get();
}

StructuralFeature& Slot::getDefiningFeatureRef() {
    return m_definingFeature.getRef();
}

ID Slot::getDefiningFeatureID() const {
    return m_definingFeature.id();
}

bool Slot::hasDefiningFeature() const {
    return m_definingFeature.has();
}

void Slot::setDefiningFeature(StructuralFeature* definingFeature) {
    m_definingFeature.set(definingFeature);
}

void Slot::setDefiningFeature(StructuralFeature& definingFeature) {
    m_definingFeature.set(definingFeature);
}

InstanceSpecification* Slot::getOwningInstance() {
    return m_owningInstance.get();
}

InstanceSpecification& Slot::getOwningInstanceRef() {
    return m_owningInstance.getRef();
}

ID Slot::getOwningInstanceID() const {
    return m_owningInstance.id();
}

bool Slot::hasOwningInstance() const {
    return m_owningInstance.has();
}

void Slot::setOwningInstance(InstanceSpecification* inst) {
    m_owningInstance.set(inst);
}

void Slot::setOwningInstance(InstanceSpecification& inst) {
    m_owningInstance.set(inst);
}

bool Slot::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if(!ret) {
        ret = eType == ElementType::SLOT;
    }

    return ret;
}