#include "uml/expression.h"
#include "uml/uml-stable.h"

using namespace UML;

Set<ValueSpecification, Expression>& Expression::getOperandsSet() {
    return m_operands;
}

void Expression::init() {
    m_operands.subsets(*m_ownedElements);
}

void Expression::copy(const Expression& rhs) {
    m_operands = rhs.m_operands;
}

Expression::Expression() : Element(ElementType::EXPRESSION) {
    init();
}

Expression::Expression(const Expression& rhs) : Element(rhs, ElementType::EXPRESSION) {
    init();
    Element::copy(rhs);
    TypedElement::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    copy(rhs);
    if (!m_copiedElementFlag) {
        delete &rhs;
    }
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