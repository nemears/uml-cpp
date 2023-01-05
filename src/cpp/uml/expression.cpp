#include "uml/expression.h"
#include "uml/behavior.h"
#include "uml/association.h"
#include "uml/dataType.h"
#include "uml/stereotype.h"
#include "uml/deployment.h"

using namespace UML;

Expression::Expression() : Element(ElementType::EXPRESSION) {
    m_operands.subsets(*m_ownedElements);
}

Expression::~Expression() {
    
}

OrderedSet<ValueSpecification, Expression>& Expression::getOperands() {
    return m_operands;
}

std::string Expression::getSymbol() {
    return m_symbol;
}

void Expression::setSymbol(std::string sym) {
    m_symbol = sym;
}

bool Expression::isSubClassOf(ElementType eType) const {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXPRESSION;
    }

    return ret;
}