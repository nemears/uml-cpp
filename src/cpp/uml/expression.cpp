#include "uml/expression.h"

using namespace std;
using namespace UML;

void Expression::AddOperandFunctor::operator()(ValueSpecification& el) const {
    if (!m_el->getOwnedElements().contains(el.getID())) {
        m_el->getOwnedElements().add(el);
    }
    if (el.getExpressionID() != m_el->getID()) {
        el.setExpression(m_el);
    }
    updateCopiedSequenceAddedTo(el, &Expression::getOperands);
}

void Expression::AddOperandFunctor::operator()(ID id) const {
    if (!m_el->getOwnedElements().contains(id)) {
        m_el->getOwnedElements().add(id);
    }
}

void Expression::RemoveOperandFunctor::operator()(ValueSpecification& el) const {
    if (m_el->getOwnedElements().contains(el.getID())) {
        m_el->getOwnedElements().remove(el);
    }
    if (el.getExpressionID() == m_el->getID()) {
        el.setExpression(m_el);
    }
    updateCopiedSequenceRemovedFrom(el, &Expression::getOperands);
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

Expression::Expression() : Element(ElementType::EXPRESSION) {
    m_operands.addProcedures.push_back(new AddOperandFunctor(this));
    m_operands.removeProcedures.push_back(new RemoveOperandFunctor(this));
}

Expression::Expression(const Expression& rhs) : Element(rhs, ElementType::EXPRESSION) {
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

bool Expression::isSubClassOf(ElementType eType) const {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXPRESSION;
    }

    return ret;
}