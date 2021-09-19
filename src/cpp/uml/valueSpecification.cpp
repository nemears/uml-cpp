#include "uml/valueSpecification.h"
#include "uml/slot.h"
#include "uml/expression.h"

using namespace std;
using namespace UML;

void ValueSpecification::RemoveOwningSlotProcedure::operator()(Slot* el) const {
    if (el->getValues().count(m_me->getID())) {
        el->getValues().remove(*m_me);
    }
}

void ValueSpecification::AddOwningSlotProcedure::operator()(Slot* el) const {
    if (!el->getValues().count(m_me->getID())) {
        el->getValues().add(*m_me);
    }
}

void ValueSpecification::reindexName(string oldName, string newName) {
    if (getOwner()) {
        if (getOwner()->isSubClassOf(ElementType::SLOT)) {
            dynamic_cast<Slot*>(getOwner())->getValues().reindex(m_id, oldName, newName);
        } else if (getOwner()->isSubClassOf(ElementType::EXPRESSION)) {
            dynamic_cast<Expression*>(getOwner())->getOperands().reindex(m_id, oldName, newName);
        }
    }

    NamedElement::reindexName(oldName, newName);
}

void ValueSpecification::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    if (m_owningSlot.id() == oldID) {
        m_owningSlot.reindex(oldID, newID);
    }
}

ValueSpecification::ValueSpecification() {
    m_owningSlot.m_signature = &ValueSpecification::m_owningSlot;
    m_owningSlot.m_removeProcedures.push_back(new RemoveOwningSlotProcedure(this));
    m_owningSlot.m_addProcedures.push_back(new AddOwningSlotProcedure(this));
}

ValueSpecification::ValueSpecification(const ValueSpecification& rhs) {
    m_owningSlot = rhs.m_owningSlot;
    m_owningSlot.m_me = this;
    m_owningSlot.m_removeProcedures.clear();
    m_owningSlot.m_addProcedures.clear();
    m_owningSlot.m_removeProcedures.push_back(new RemoveOwningSlotProcedure(this));
    m_owningSlot.m_addProcedures.push_back(new AddOwningSlotProcedure(this));
}

Slot* ValueSpecification::getOwningSlot() {
    return m_owningSlot.get();
}

Slot& ValueSpecification::getOwningSlotRef() {
    return m_owningSlot.getRef();
}

bool ValueSpecification::hasOwningSlot() const {
    return m_owningSlot.has();
}

void ValueSpecification::setOwningSlot(Slot* slot) {
    m_owningSlot.set(slot);
}

void ValueSpecification::setOwningSlot(Slot& slot) {
    m_owningSlot.set(slot);
}

ElementType ValueSpecification::getElementType() const {
    return ElementType::VALUE_SPECIFICATION;
}

bool ValueSpecification::isSubClassOf(ElementType eType) const {
    bool ret = TypedElement::isSubClassOf(eType);

    if (!ret) {
        ret = PackageableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::VALUE_SPECIFICATION;
    }

    return ret;
}

void ValueSpecification::restoreReleased(ID id, Element* el) {
    PackageableElement::restoreReleased(id, el);
    TypedElement::restoreReleased(id, el);
}

void ValueSpecification::referencingReleased(ID id) {
    PackageableElement::referencingReleased(id);
    TypedElement::referencingReleased(id);
    if (m_owningSlot.id() == id) {
        m_owningSlot.release();
    }
}