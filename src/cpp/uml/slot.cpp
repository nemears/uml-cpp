#include "uml/slot.h"
#include "uml/instanceSpecification.h"

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

void Slot::AddValueFunctor::operator()(Element& el) const {
    if (dynamic_cast<ValueSpecification&>(el).getOwningSlot() != m_el) {
        dynamic_cast<ValueSpecification&>(el).setOwningSlot(dynamic_cast<Slot*>(m_el));
    }
    
    if (el.getOwner() != m_el) {
        el.setOwner(m_el);
    }
}

void Slot::RemoveValueFunctor::operator()(Element& el) const {
    if (dynamic_cast<ValueSpecification&>(el).getOwningSlot() == m_el) {
        dynamic_cast<ValueSpecification&>(el).setOwningSlot(0);
    }

    if (dynamic_cast<ValueSpecification&>(el).getOwner() == m_el) {
        dynamic_cast<ValueSpecification&>(el).setOwner(0);
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
    if (!m_definingFeatureID.isNull()) {
        if (!m_definingFeaturePtr) {
            m_definingFeaturePtr = &m_manager->get<StructuralFeature>(m_definingFeatureID);
        }
        return m_definingFeaturePtr;
    }
    return 0;
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
    if (!m_owningInstanceID.isNull()) {
        if (!m_owningInstancePtr) {
            m_owningInstancePtr = &m_manager->get<InstanceSpecification>(m_owningInstanceID);
        }
        return m_owningInstancePtr;
    }
    return 0;
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

bool Slot::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if(!ret) {
        ret = eType == ElementType::SLOT;
    }

    return ret;
}