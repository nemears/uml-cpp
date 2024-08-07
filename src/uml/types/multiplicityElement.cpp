#include "uml/types/multiplicityElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void MultiplicityElement::LowerPolicy::elementAdded(ValueSpecification& el, MultiplicityElement& me) {
    if (el.is(ElementType::LITERAL_INT)) {
        if (dynamic_cast<LiteralInt&>(el).getValue() >= 0) {
            me.setLower(dynamic_cast<LiteralInt&>(el).getValue());
        }
    }
    else if (el.is(ElementType::EXPRESSION)) {
        // TODO evaluate expression
    }
}

void MultiplicityElement::LowerPolicy::elementRemoved(__attribute__((unused)) ValueSpecification& el, MultiplicityElement& me) {
    if (me.m_lowSpecified) {
        me.m_lower = -1;
        me.m_lowSpecified = false;
        me.m_multiplicityIsSpecified = false;
    }
}

void MultiplicityElement::UpperPolicy::elementAdded(ValueSpecification& el, MultiplicityElement& me) {
    if (el.is(ElementType::LITERAL_INT)) {
        if (dynamic_cast<LiteralInt&>(el).getValue() >= 0) {
            me.setUpper(dynamic_cast<LiteralInt&>(el).getValue());
        }
    }
    else if (el.is(ElementType::EXPRESSION)) {
        // TODO evaluate expression
    }
}

void MultiplicityElement::UpperPolicy::elementRemoved(__attribute__((unused)) ValueSpecification& el, MultiplicityElement& me) {
    if (me.m_upSpecified) {
        me.m_upper = -1;
        me.m_upSpecified = false;
        me.m_multiplicityIsSpecified = false;
    }
}

Singleton<ValueSpecification, MultiplicityElement, MultiplicityElement::LowerPolicy>& MultiplicityElement::getLowerValueSingleton() {
    return m_lowVal;
}

Singleton<ValueSpecification, MultiplicityElement, MultiplicityElement::UpperPolicy>& MultiplicityElement::getUpperValueSingleton() {
    return m_upVal;
}

MultiplicityElement::MultiplicityElement() : Element(ElementType::MULTIPLICITY_ELEMENT) {
    m_lowVal.subsets(*m_ownedElements);
    m_upVal.subsets(*m_ownedElements);
}

MultiplicityElement::~MultiplicityElement() {
    
}

int MultiplicityElement::getLower() {
    return m_lower;
    // if (m_lowSpecified) {
    //     return m_lower;
    // } else {
    //     // throw multiplicityNotSpecifiedException;
    // }
}

void MultiplicityElement::setLower(const int low) {
    m_lower = low;
    if (!m_lowSpecified) {
        m_lowSpecified = true;
    }
    if (m_upSpecified) {
        m_multiplicityIsSpecified = true;
    }
}

int MultiplicityElement::getUpper() {
    return m_upper;
    // if (m_upSpecified) {
    //     return m_upper;
    // } else {
    //     // throw multiplicityNotSpecifiedException;
    // }
}

void MultiplicityElement::setUpper(const int up) {
    m_upper = up;
    if (!m_upSpecified) {
        m_upSpecified = true;
    }
    if (m_lowSpecified) {
        m_multiplicityIsSpecified = true;
    }
}

bool MultiplicityElement::multiplicitySpecified() {
    return m_multiplicityIsSpecified;
}

bool MultiplicityElement::lowSpecified() const {
    return m_lowSpecified;
}
bool MultiplicityElement::upSpecified() const {
    return m_upSpecified;
}

ValueSpecificationPtr MultiplicityElement::getLowerValue() const {
    return m_lowVal.get();
}

void MultiplicityElement::setLowerValue(ValueSpecification* val) {
    m_lowVal.set(val);
}

void MultiplicityElement::setLowerValue(ValueSpecification& val) {
    m_lowVal.set(val);
}

void MultiplicityElement::setLowerValue(ValueSpecificationPtr val) {
    m_lowVal.set(val);
}

void MultiplicityElement::setLowerValue(ID id) {
    m_lowVal.set(id);
}

ValueSpecificationPtr MultiplicityElement::getUpperValue() const {
    return m_upVal.get();
}

void MultiplicityElement::setUpperValue(ValueSpecification* val) {
    m_upVal.set(val);
}

void MultiplicityElement::setUpperValue(ValueSpecification& val) {
    m_upVal.set(val);
}

void MultiplicityElement::setUpperValue(ValueSpecificationPtr val) {
    m_upVal.set(val);
}

void MultiplicityElement::setUpperValue(ID id) {
    m_upVal.set(id);
}

bool MultiplicityElement::isOrdered() const {
    return m_isOrdered;
}

void MultiplicityElement::setIsOrdered(bool isOrdered) {
    m_isOrdered = isOrdered;
}

bool MultiplicityElement::isUnique() const {
    return m_isUnique;
}

void MultiplicityElement::setIsUnique(bool isUnique) {
    m_isUnique = isUnique;
}

bool MultiplicityElement::is(ElementType eType) const {
    bool ret = Element::is(eType);

    if (!ret) {
        ret = eType == ElementType::MULTIPLICITY_ELEMENT;
    }

    return ret;
}
