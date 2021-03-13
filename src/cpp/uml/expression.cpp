#include "uml/expression.h"

using namespace UML;

Expression::Expression() {
    m_symbol = "";
    m_operands = new Sequence<ValueSpecification>;
}

Expression::~Expression() {
    delete m_operands;
}

Sequence<ValueSpecification>& Expression::getOperands() {
    return *m_operands;
}

string Expression::getSymbol() {
    return m_symbol;
}

void Expression::setSymbol(string sym) {
    m_symbol = sym;
}

ElementType Expression::getElementType() {
    return ElementType::EXPRESSION;
}