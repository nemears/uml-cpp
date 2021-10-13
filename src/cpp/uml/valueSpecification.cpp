#include "uml/valueSpecification.h"
#include "uml/slot.h"
#include "uml/expression.h"
#include "uml/instanceSpecification.h"

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
    if (m_me->getOwnerID() != el->getID()) {
        m_me->setOwner(el);
    }
}

void ValueSpecification::AddOwningSlotProcedure::operator()(ID id) const {
    if (m_me->getOwnerID() != id) {
        m_me->setOwnerByID(id);
    }
}

void ValueSpecification::RemoveOwningInstanceSpecProcedure::operator()(InstanceSpecification* el) const {
    if (el->getSpecificationID() == m_me->getID()) {
        el->setSpecification(0);
    }
    if (m_me->getOwnerID() == el->getID()) {
        m_me->setOwner(0);
    }
}

void ValueSpecification::AddOwningInstanceSpecProcedure::operator()(InstanceSpecification* el) const {
    if (el->getSpecificationID() != m_me->getID()) {
        el->setSpecification(m_me);
    }
    if (m_me->getOwnerID() != el->getID()) {
        m_me->setOwner(el);
    }
}

void ValueSpecification::AddOwningInstanceSpecProcedure::operator()(ID id) const {
    if (m_me->getOwnerID() != id) {
        m_me->setOwnerByID(id);
    }
}

void ValueSpecification::RemoveExpressionProcedure::operator()(Expression* el) const {
    if (el->getOperands().count(m_me->getID())) {
        el->getOperands().remove(*m_me);
    }
    if (m_me->getExpressionID() == el->getID()) {
        m_me->setExpression(0);
    }
}

void ValueSpecification::AddExpressionProcedure::operator()(Expression* el) const {
    if (!el->getOperands().count(m_me->getID())) {
        el->getOperands().add(*m_me);
    }
    if (m_me->getOwnerID() != el->getID()) {
        m_me->setOwner(el);
    }
}

void ValueSpecification::AddExpressionProcedure::operator()(ID id) const {
    if (m_me->getOwnerID() != id) {
        m_me->setOwnerByID(id);
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
    if (m_owningInstanceSpec.id() == oldID) {
        m_owningInstanceSpec.reindex(oldID, newID);
    }
    if (m_expression.id() == oldID) {
        m_expression.reindex(oldID, newID);
    }
}

void ValueSpecification::restoreReferences() {
    TypedElement::restoreReferences();
    PackageableElement::restoreReferences();
    m_owningSlot.restoreReference();
    m_owningInstanceSpec.restoreReference();
    m_expression.restoreReference();
}

ValueSpecification::ValueSpecification() {
    m_owningSlot.m_signature = &ValueSpecification::m_owningSlot;
    m_owningSlot.m_removeProcedures.push_back(new RemoveOwningSlotProcedure(this));
    m_owningSlot.m_addProcedures.push_back(new AddOwningSlotProcedure(this));
    m_owningInstanceSpec.m_signature = &ValueSpecification::m_owningInstanceSpec;
    m_owningInstanceSpec.m_removeProcedures.push_back(new RemoveOwningInstanceSpecProcedure(this));
    m_owningInstanceSpec.m_addProcedures.push_back(new AddOwningInstanceSpecProcedure(this));
    m_expression.m_signature = &ValueSpecification::m_expression;
    m_expression.m_addProcedures.push_back(new AddExpressionProcedure(this));
    m_expression.m_removeProcedures.push_back(new RemoveExpressionProcedure(this));
}

ValueSpecification::ValueSpecification(const ValueSpecification& rhs) {
    m_owningSlot = rhs.m_owningSlot;
    m_owningSlot.m_me = this;
    m_owningSlot.m_removeProcedures.clear();
    m_owningSlot.m_addProcedures.clear();
    m_owningSlot.m_removeProcedures.push_back(new RemoveOwningSlotProcedure(this));
    m_owningSlot.m_addProcedures.push_back(new AddOwningSlotProcedure(this));
    m_owningInstanceSpec = rhs.m_owningInstanceSpec;
    m_owningInstanceSpec.m_me = this;
    m_owningInstanceSpec.m_removeProcedures.clear();
    m_owningInstanceSpec.m_addProcedures.clear();
    m_owningInstanceSpec.m_removeProcedures.push_back(new RemoveOwningInstanceSpecProcedure(this));
    m_owningInstanceSpec.m_addProcedures.push_back(new AddOwningInstanceSpecProcedure(this));
    m_expression = rhs.m_expression;
    m_expression.m_me = this;
    m_expression.m_addProcedures.clear();
    m_expression.m_removeProcedures.clear();
    m_expression.m_addProcedures.push_back(new AddExpressionProcedure(this));
    m_expression.m_removeProcedures.push_back(new RemoveExpressionProcedure(this));
}

Slot* ValueSpecification::getOwningSlot() {
    return m_owningSlot.get();
}

Slot& ValueSpecification::getOwningSlotRef() {
    return m_owningSlot.getRef();
}

ID ValueSpecification::getOwningSlotID() const {
    return m_owningSlot.id();
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

InstanceSpecification* ValueSpecification::getOwningInstanceSpec() {
    return m_owningInstanceSpec.get();
}

InstanceSpecification& ValueSpecification::getOwningInstanceSpecRef() {
    return m_owningInstanceSpec.getRef();
}

ID ValueSpecification::getOwningInstanceSpecID() const {
    return m_owningInstanceSpec.id();
}

bool ValueSpecification::hasOwningInstanceSpec() const {
    return m_owningInstanceSpec.has();
}

void ValueSpecification::setOwningInstanceSpec(InstanceSpecification* instanceSpec) {
    m_owningInstanceSpec.set(instanceSpec);
}

void ValueSpecification::setOwningInstanceSpec(InstanceSpecification& instanceSpec) {
    m_owningInstanceSpec.set(instanceSpec);
}

Expression* ValueSpecification::getExpression() {
    return m_expression.get();
}

Expression& ValueSpecification::getExpressionRef() {
    return m_expression.getRef();
}

ID ValueSpecification::getExpressionID() const {
    return m_expression.id();
}

bool ValueSpecification::hasExpression() const {
    return m_expression.has();
}

void ValueSpecification::setExpression(Expression* expression) {
    m_expression.set(expression);
}

void ValueSpecification::setExpression(Expression& expression) {
    m_expression.set(expression);
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

void ValueSpecification::referencingReleased(ID id) {
    PackageableElement::referencingReleased(id);
    TypedElement::referencingReleased(id);
    if (m_owningSlot.id() == id) {
        m_owningSlot.release();
    }
}