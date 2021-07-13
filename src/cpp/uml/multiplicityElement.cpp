#include "uml/multiplicityElement.h"
#include "uml/literalInt.h"
#include "uml/valueSpecification.h"

using namespace UML;

MultiplicityElement::MultiplicityElement() {
    m_lowValPtr = 0;
    m_upValPtr = 0;
}

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
    if (!m_lowValID.isNull()) {
        if (!m_lowValPtr) {
            m_lowValPtr = &m_manager->get<ValueSpecification>(m_lowValID);
        }
        return m_lowValPtr;
    }
    return 0;
}

void MultiplicityElement::setLowerValue(ValueSpecification* val) {
    // evaluate
    if (val->isSubClassOf(ElementType::LITERAL_INT)) {
        if (dynamic_cast<LiteralInt*>(val)->getValue() >= 0) {
            setLower(dynamic_cast<LiteralInt*>(val)->getValue());
        }
    } else if (val->isSubClassOf(ElementType::EXPRESSION)) {
        // TODO evaluate expression
    }

    if (val) {
        m_lowValID = val->getID();
    } else {
        m_lowValID = ID::nullID();
    }

    if (!m_manager) {
        m_lowValPtr = val;
    }
    // TODO error
}

ValueSpecification* MultiplicityElement::getUpperValue() {
    if (!m_upValID.isNull()) {
        if (!m_upValPtr) {
            m_upValPtr = &m_manager->get<ValueSpecification>(m_upValID);
        }
        return m_upValPtr;
    }
    return 0;
}

void MultiplicityElement::setUpperValue(ValueSpecification* val) {
    if (val->isSubClassOf(ElementType::LITERAL_INT)) {
        if (dynamic_cast<LiteralInt*>(val)->getValue() >= 0) {
            setUpper(dynamic_cast<LiteralInt*>(val)->getValue());
        }
    }

    if (val) {
        m_upValID = val->getID();
    } else {
        m_upValID = ID::nullID();
    }

    if (!m_manager) {
        m_upValPtr = val;
    }
}

ElementType MultiplicityElement::getElementType() const {
    return ElementType::MULTIPLICITY_ELEMENT;
}

bool MultiplicityElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MULTIPLICITY_ELEMENT;
    }

    return ret;
}