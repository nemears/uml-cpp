#include "uml/valueSpecification.h"
#include "uml/slot.h"
#include "uml/expression.h"

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

ElementType ValueSpecification::getElementType() {
    return ElementType::VALUE_SPECIFICATION;
}