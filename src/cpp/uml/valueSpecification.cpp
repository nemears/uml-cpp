#include "uml/valueSpecification.h"
#include "uml/slot.h"
#include "uml/expression.h"

using namespace std;
using namespace UML;

void ValueSpecification::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_owner) {
        if (m_owner->isSubClassOf(ElementType::SLOT)) {
            dynamic_cast<Slot*>(m_owner)->getValues().reindex(oldID, newID);
        } else if (m_owner->isSubClassOf(ElementType::EXPRESSION)) {
            dynamic_cast<Expression*>(m_owner)->getOperands().reindex(oldID, newID);
        }
    }

    NamedElement::reindexID(oldID, newID);
}

void ValueSpecification::reindexName(string oldName, string newName) {
    if (m_owner) {
        if (m_owner->isSubClassOf(ElementType::SLOT)) {
            dynamic_cast<Slot*>(m_owner)->getValues().reindex(m_id, oldName, newName);
        } else if (m_owner->isSubClassOf(ElementType::EXPRESSION)) {
            dynamic_cast<Expression*>(m_owner)->getOperands().reindex(m_id, oldName, newName);
        }
    }

    NamedElement::reindexName(oldName, newName);
}

ValueSpecification::ValueSpecification() {
    m_owningSlot = 0;
}

Slot* ValueSpecification::getOwningSlot() {
    return m_owningSlot;
}

void ValueSpecification::setOwningSlot(Slot* slot) {
    if (m_owningSlot) {
        if (m_owningSlot != slot) {
            if (m_owningSlot->getValues().count(m_id)) {
                m_owningSlot->getValues().remove(*this);
            }
        }
    }
    m_owningSlot = slot;
    if (m_owningSlot) {
        if (!m_owningSlot->getValues().count(m_id)) {
            m_owningSlot->getValues().add(*this);
        }
    }
}

ElementType ValueSpecification::getElementType() const {
    return ElementType::VALUE_SPECIFICATION;
}

bool ValueSpecification::isSubClassOf(ElementType eType) {
    bool ret = TypedElement::isSubClassOf(eType);

    if (!ret) {
        ret = PackageableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::VALUE_SPECIFICATION;
    }

    return ret;
}