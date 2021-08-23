#include "uml/slot.h"
#include "uml/instanceSpecification.h"
#include "uml/structuralFeature.h"
#include "uml/valueSpecification.h"
#include "uml/universalFunctions.h"

using namespace UML;

void Slot::reindexID(ID oldID, ID newID) {
    if (!m_owningInstanceID.isNull()) {
        if (!m_owningInstancePtr) {
            m_owningInstancePtr = &m_manager->get<InstanceSpecification>(m_owningInstanceID);
        }
        m_owningInstancePtr->getSlots().reindex(oldID, newID);
    }
    
    Element::reindexID(oldID, newID);
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
    m_definingFeaturePtr = 0;
    m_values.addProcedures.push_back(new AddValueFunctor(this));
    m_values.removeProcedures.push_back(new RemoveValueFunctor(this));
    m_owningInstancePtr = 0;
}

Slot::~Slot() {
    
}

Sequence<ValueSpecification>& Slot::getValues() {
    return m_values;
}

StructuralFeature* Slot::getDefiningFeature() {
    return universalGet<StructuralFeature>(m_definingFeatureID, m_definingFeaturePtr, m_manager);
}

void Slot::setDefiningFeature(StructuralFeature* definingFeature) {
    if (definingFeature) {
        m_definingFeatureID = definingFeature->getID();
    }
    
    if (!m_manager) {
        m_definingFeaturePtr = definingFeature;
    }
}

InstanceSpecification* Slot::getOwningInstance() {
    return universalGet<InstanceSpecification>(m_owningInstanceID, m_owningInstancePtr, m_manager);
}

void Slot::setOwningInstance(InstanceSpecification* inst) {
    if (!isSameOrNull(m_owningInstanceID, inst)) {
        if (!m_owningInstancePtr) {
            m_owningInstancePtr = &m_manager->get<InstanceSpecification>(m_owningInstanceID);
        }
        if (m_owningInstancePtr->getSlots().count(m_id)) {
            m_owningInstancePtr->getSlots().remove(*this);
        }
        m_owningInstanceID = ID::nullID();
        m_owningInstancePtr = 0;
    }

    if (inst) {
        m_owningInstanceID = inst->getID();
    }

    if (!m_manager) {
        m_owningInstancePtr = inst;
    }

    if (inst) {
        if (!inst->getSlots().count(m_id)) {
            inst->getSlots().add(*this);
        }
    }
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