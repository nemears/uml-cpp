#include "uml/slot.h"
#include "uml/instanceSpecification.h"

using namespace UML;

void Slot::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_owningInstance) {
        m_owningInstance->getSlots().reindex(oldID, newID);
    }
    
    Element::reindexID(oldID, newID);
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
    m_definingFeature = 0;
    m_values = new Sequence<ValueSpecification>;
    m_values->addProcedures.push_back(new AddValueFunctor(this));
    m_values->removeProcedures.push_back(new RemoveValueFunctor(this));
    m_owningInstance = NULL;
}

Slot::~Slot() {
    delete m_values;
}

Sequence<ValueSpecification>& Slot::getValues() {
    return *m_values;
}

StructuralFeature* Slot::getDefiningFeature() {
    return m_definingFeature;
}

void Slot::setDefiningFeature(StructuralFeature* definingFeature) {
    m_definingFeature = definingFeature;
}

InstanceSpecification* Slot::getOwningInstance() {
    return m_owningInstance;
}

void Slot::setOwningInstance(InstanceSpecification* inst) {
    m_owningInstance = inst;
    if (m_owningInstance) {
        if (!m_owningInstance->getSlots().count(m_id)) {
            m_owningInstance->getSlots().add(*this);
        }
    }
}

ElementType Slot::getElementType() {
    return ElementType::SLOT;
}

bool Slot::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if(!ret) {
        ret = eType == ElementType::SLOT;
    }

    return ret;
}