#include "uml/expression.h"
#include "uml/behavior.h"
#include "uml/association.h"
#include "uml/dataType.h"
#include "uml/stereotype.h"
#include "uml/deployment.h"

using namespace UML;

Set<ValueSpecification, Expression>& Expression::getOperandsSet() {
    return m_operands;
}

void Expression::init() {
    m_operands.subsets(*m_ownedElements);
}

Expression::Expression() : Element(ElementType::EXPRESSION) {
    init();
}

Expression::~Expression() {
    mountAndRelease();
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