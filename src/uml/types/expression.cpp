#include "uml/types/expression.h"
#include "uml/types/behavior.h"
#include "uml/types/association.h"
#include "uml/types/dataType.h"
#include "uml/types/stereotype.h"
#include "uml/types/deployment.h"

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