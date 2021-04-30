#include "uml/multiplicityElement.h"
#include "uml/literalInt.h"

using namespace UML;

int MultiplicityElement::getLower() {
    if (lowSpecified) {
        return lower;
    } else {
        throw multiplicityNotSpecifiedException;
    }
}

void MultiplicityElement::setLower(const int low) {
    lower = low;
    if (!lowSpecified) {
        lowSpecified = true;
    }
    if (upSpecified) {
        multiplicityIsSpecified = true;
    }
}

int MultiplicityElement::getUpper() {
    if (upSpecified) {
        return upper;
    } else {
        throw multiplicityNotSpecifiedException;
    }
}

void MultiplicityElement::setUpper(const int up) {
    upper = up;
    if (!upSpecified) {
        upSpecified = true;
    }
    if (lowSpecified) {
        multiplicityIsSpecified = true;
    }
}

bool MultiplicityElement::multiplicitySpecified() {
    return multiplicityIsSpecified;
}

ValueSpecification* MultiplicityElement::getLowerValue() {
    return m_lowVal;
}

void MultiplicityElement::setLowerValue(ValueSpecification* val) {
    // evaluate
    if (val->isSubClassOf(ElementType::LITERAL_INT)) {
        if (dynamic_cast<LiteralInt*>(val)->getValue() >= 0) {
            setLower(dynamic_cast<LiteralInt*>(val)->getValue());
            m_lowVal = val;
            return;
        }
    } else if (val->isSubClassOf(ElementType::EXPRESSION)) {
        // TODO evaluate expression
    }
    // TODO error
}

ValueSpecification* MultiplicityElement::getUpperValue() {
    return m_upVal;
}

void MultiplicityElement::setUpperValue(ValueSpecification* val) {
    if (val->isSubClassOf(ElementType::LITERAL_INT)) {
        if (dynamic_cast<LiteralInt*>(val)->getValue() >= 0) {
            setUpper(dynamic_cast<LiteralInt*>(val)->getValue());
            m_upVal = val;
            return;
        }
    }
}

ElementType MultiplicityElement::getElementType() const {
    return ElementType::MULTIPLICITY_ELEMENT;
}

bool MultiplicityElement::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MULTIPLICITY_ELEMENT;
    }

    return ret;
}