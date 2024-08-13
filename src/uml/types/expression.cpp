#include "uml/uml-stable.h"

using namespace UML;

Expression::Expression() : Element(ElementType::EXPRESSION) {
    m_operands.subsets(*m_ownedElements);
}

Expression::~Expression() {
    
}

IndexableOrderedSet<ValueSpecification, Expression>& Expression::getOperands() {
    return m_operands;
}

std::string Expression::getSymbol() {
    return m_symbol;
}

void Expression::setSymbol(const std::string& sym) {
    m_symbol = sym;
}

bool Expression::is(ElementType eType) const {
    bool ret = ValueSpecification::is(eType);

    if (!ret) {
        ret = eType == ElementType::EXPRESSION;
    }

    return ret;
}
