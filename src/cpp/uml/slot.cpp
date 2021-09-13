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
}

void Slot::reindexID(ID oldID, ID newID) {
    /**if (!m_owningInstanceID.isNull()) {
        if (!m_owningInstancePtr) {
            m_owningInstancePtr = &m_manager->get<InstanceSpecification>(m_owningInstanceID);
        }
        m_owningInstancePtr->getSlots().reindex(oldID, newID);
    }
    
    Element::reindexID(oldID, newID);**/
}

void Slot::restoreReleased(ID id, Element* released) {
    Element::restoreReleased(id, released);
}

void Slot::referencingReleased(ID id) {
    Element::referencingReleased(id);
    if (m_definingFeature.id() == id) {
        m_definingFeature.release();
    }
    if (m_values.count(id)) {
        m_values.elementReleased(id, &Slot::getValues);
    }
    if (m_owningInstance.id() == id) {
        m_owningInstance.release();
    }
}

void Slot::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    if (m_definingFeature.id() == oldID) {
        m_definingFeature.reindex(oldID, newID);
    }
    if (m_values.count(oldID)) {
        m_values.reindex(oldID, newID, &Slot::getValues);
    }
    if (m_owningInstance.id() == oldID) {
        m_owningInstance.reindex(oldID, newID);
    }
}

void Slot::setManager(UmlManager* manager) {
    Element::setManager(manager);
    m_values.m_manager = manager;
}

void Slot::AddValueFunctor::operator()(ValueSpecification& el) const {
    if (el.getOwningSlot() != m_el) {
        el.setOwningSlot(m_el);
    }
    
    if (el.getOwner() != m_el) {
        el.setOwner(m_el);
    }
}

void Slot::RemoveValueFunctor::operator()(ValueSpecification& el) const {
    if (el.getOwningSlot() == m_el) {
        el.setOwningSlot(0);
    }

    if (el.getOwner() == m_el) {
        el.setOwner(0);
    }
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