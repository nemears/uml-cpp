#include "uml/expression.h"

using namespace std;
using namespace UML;

void Expression::AddOperandFunctor::operator()(Element& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().add(el);
    }
}

Expression::Expression() {
    m_symbol = "";
    m_operands.addProcedures.push_back(new AddOperandFunctor(this));
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

bool Expression::isSubClassOf(ElementType eType) {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXPRESSION;
    }

    return ret;
}