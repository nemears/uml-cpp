#include "uml/uml-stable.h"

using namespace UML;

Expression::Expression(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    ValueSpecification(elementType, manager) 
{
    m_operands.subsets(m_ownedElements);
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
