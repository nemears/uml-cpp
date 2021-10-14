#include "uml/expression.h"

using namespace std;
using namespace UML;

void Expression::AddOperandFunctor::operator()(ValueSpecification& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
    if (el.getExpressionID() != m_el->getID()) {
        el.setExpression(m_el);
    }
    updateCopiedSequenceAddedTo(el, &Expression::getOperands);
}

void Expression::AddOperandFunctor::operator()(ID id) const {
    if (!m_el->getOwnedElements().count(id)) {
        m_el->getOwnedElements().addByID(id);
    }
}

void Expression::RemoveOperandFunctor::operator()(ValueSpecification& el) const {
    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().remove(el);
    }
    if (el.getExpressionID() == m_el->getID()) {
        el.setExpression(m_el);
    }
    updateCopiedSequenceRemovedFrom(el, &Expression::getOperands);
}

void Expression::setManager(UmlManager* manager) {
    ValueSpecification::setManager(manager);
    m_operands.m_manager = manager;
}

void Expression::referencingReleased(ID id) {
    ValueSpecification::referencingReleased(id);
    m_operands.elementReleased(id, &Expression::getOperands);
}

void Expression::referenceReindexed(ID oldID, ID newID) {
    ValueSpecification::referenceReindexed(oldID, newID);
    m_operands.reindex(oldID, newID, &Expression::getOperands);
}

void Expression::referenceErased(ID id) {
    ValueSpecification::referenceErased(id);
    m_operands.elementErased(id);
}

Expression::Expression() {
    m_operands.addProcedures.push_back(new AddOperandFunctor(this));
    m_operands.removeProcedures.push_back(new RemoveOperandFunctor(this));
}

Expression::Expression(const Expression& rhs) {
    m_operands = rhs.m_operands;
    m_operands.m_el = this;
    m_operands.addProcedures.clear();
    m_operands.removeProcedures.clear();
    m_operands.addProcedures.push_back(new AddOperandFunctor(this));
    m_operands.removeProcedures.push_back(new RemoveOperandFunctor(this));
}

Expression::~Expression() {
    
}

Sequence<ValueSpecification>& Expression::getOperands() {
    return m_operands;
}

string Expression::getSymbol() {
    return m_symbol;
}

void Expression::setSymbol(string sym) {
    m_symbol = sym;
}

ElementType Expression::getElementType() const {
    return ElementType::EXPRESSION;
}

bool Expression::isSubClassOf(ElementType eType) const {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXPRESSION;
    }

    return ret;
}