#include "uml/slot.h"
#include "uml/instanceSpecification.h"
#include "uml/structuralFeature.h"
#include "uml/valueSpecification.h"

using namespace UML;

void Slot::RemoveDefiningFeatureProcedure::operator()(StructuralFeature* el) const {
    el->removeReference(m_me->getID());
}

void Slot::AddDefiningFeatureProcedure::operator()(StructuralFeature* el) const {
    el->setReference(m_me);
}

void Slot::RemoveOwningInstanceProcedure::operator()(InstanceSpecification* el) const {
    if (el->getSlots().count(m_me->getID())) {
        el->getSlots().remove(*m_me);
    }
}

void Slot::AddOwningInstanceProcedure::operator()(InstanceSpecification* el) const {
    if (!el->getSlots().count(m_me->getID())) {
        el->getSlots().add(*m_me);
    }
    if (m_me->getOwnerID() != el->getID()) {
        m_me->setOwner(el);
    }
}

void Slot::AddOwningInstanceProcedure::operator()(ID id) const {
    if (m_me->getOwnerID() != id) {
        m_me->setOwnerByID(id);
    }
}

void Slot::AddValueFunctor::operator()(ValueSpecification& el) const {
    if (el.getOwningSlot() != m_el) {
        el.setOwningSlot(m_el);
    }

    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
    
    if (el.getOwner() != m_el) {
        el.setOwner(m_el);
    }
    updateCopiedSequenceAddedTo(el, &Slot::getValues);
}

void Slot::AddValueFunctor::operator()(ID id) const {
    if (!m_el->getOwnedElements().count(id)) {
        m_el->getOwnedElements().addByID(id);
    }
}

void Slot::RemoveValueFunctor::operator()(ValueSpecification& el) const {
    if (el.getOwningSlot() == m_el) {
        el.setOwningSlot(0);
    }

    if (el.getOwner() == m_el) {
        el.setOwner(0);
    }
    updateCopiedSequenceRemovedFrom(el, &Slot::getValues);
}

void Slot::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_definingFeature.release(id);
    m_values.elementReleased(id, &Slot::getValues);
    m_owningInstance.release(id);
}

void Slot::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_definingFeature.reindex(oldID, newID);
    m_values.reindex(oldID, newID, &Slot::getValues);
    m_owningInstance.reindex(oldID, newID);
}

void Slot::restoreReferences() {
    Element::restoreReferences();
    m_definingFeature.restoreReference();
    m_values.restoreReferences();
    m_owningInstance.restoreReference();
}

void Slot::referenceErased(ID id) {
    Element::referenceErased(id);
    m_definingFeature.elementErased(id);
    m_values.elementErased(id);
    m_owningInstance.elementErased(id);
}

Slot::Slot() {
    m_definingFeature.m_signature = &Slot::m_definingFeature;
    m_definingFeature.m_addProcedures.push_back(new AddDefiningFeatureProcedure(this));
    m_definingFeature.m_removeProcedures.push_back(new RemoveDefiningFeatureProcedure(this));
    m_values.addProcedures.push_back(new AddValueFunctor(this));
    m_values.removeProcedures.push_back(new RemoveValueFunctor(this));
    m_owningInstance.m_signature = &Slot::m_owningInstance;
    m_owningInstance.m_removeProcedures.push_back(new RemoveOwningInstanceProcedure(this));
    m_owningInstance.m_addProcedures.push_back(new AddOwningInstanceProcedure(this));
}

Slot::Slot(const Slot& rhs) : Element(rhs) {
    m_definingFeature = rhs.m_definingFeature;
    m_definingFeature.m_me = this;
    m_definingFeature.m_removeProcedures.clear();
    m_definingFeature.m_addProcedures.clear();
    m_definingFeature.m_addProcedures.push_back(new AddDefiningFeatureProcedure(this));
    m_definingFeature.m_removeProcedures.push_back(new RemoveDefiningFeatureProcedure(this));
    m_values = rhs.m_values;
    m_values.m_el = this;
    m_values.addProcedures.clear();
    m_values.removeProcedures.clear();
    m_values.addProcedures.push_back(new AddValueFunctor(this));
    m_values.removeProcedures.push_back(new RemoveValueFunctor(this));
    m_owningInstance = rhs.m_owningInstance;
    m_owningInstance.m_me = this;
    m_owningInstance.m_removeProcedures.clear();
    m_owningInstance.m_addProcedures.clear();
    m_owningInstance.m_removeProcedures.push_back(new RemoveOwningInstanceProcedure(this));
    m_owningInstance.m_addProcedures.push_back(new AddOwningInstanceProcedure(this));
}

Slot::~Slot() {

}

Sequence<ValueSpecification>& Slot::getValues() {
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

ElementType Slot::getElementType() const {
    return ElementType::SLOT;
}

bool Slot::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if(!ret) {
        ret = eType == ElementType::SLOT;
    }

    return ret;
}