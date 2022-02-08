#include "uml/multiplicityElement.h"
#include "uml/literalInt.h"
#include "uml/valueSpecification.h"
#include "uml/uml-stable.h"

using namespace UML;

void MultiplicityElement::AddLowerFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::LITERAL_INT)) {
        if (dynamic_cast<LiteralInt&>(el).getValue() >= 0) {
            m_el.as<MultiplicityElement>().setLower(dynamic_cast<LiteralInt&>(el).getValue());
        }
    }
    else if (el.isSubClassOf(ElementType::EXPRESSION)) {
        // TODO evaluate expression
    }
}

void MultiplicityElement::RemoveLowerFunctor::operator()(Element& el) const {
    MultiplicityElement& m_me = m_el.as<MultiplicityElement>();
    if (m_me.m_lowSpecified) {
        m_me.m_lower = -1;
        m_me.m_lowSpecified = false;
        m_me.m_multiplicityIsSpecified = false;
    }
    m_me.updateCopiesScalar(-1, &MultiplicityElement::m_lower);
    m_me.updateCopiesScalar(false, &MultiplicityElement::m_lowSpecified);
    m_me.updateCopiesScalar(false, &MultiplicityElement::m_multiplicityIsSpecified);
}

void MultiplicityElement::AddUpperFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::LITERAL_INT)) {
        if (dynamic_cast<LiteralInt&>(el).getValue() >= 0) {
            m_el.as<MultiplicityElement>().setUpper(dynamic_cast<LiteralInt&>(el).getValue());
        }
    }
    else if (el.isSubClassOf(ElementType::EXPRESSION)) {
        // TODO evaluate expression
    }
}

void MultiplicityElement::RemoveUpperFunctor::operator()(Element& el) const {
    MultiplicityElement& m_me = m_el.as<MultiplicityElement>();
    if (m_me.m_upSpecified) {
        m_me.m_upper = -1;
        m_me.m_upSpecified = false;
        m_me.m_multiplicityIsSpecified = false;
    }
    m_me.updateCopiesScalar(-1, &MultiplicityElement::m_lower);
    m_me.updateCopiesScalar(false, &MultiplicityElement::m_lowSpecified);
    m_me.updateCopiesScalar(false, &MultiplicityElement::m_multiplicityIsSpecified);
}

Set<ValueSpecification, MultiplicityElement>& MultiplicityElement::getLowerValueSingleton() {
    return m_lowVal;
}

Set<ValueSpecification, MultiplicityElement>& MultiplicityElement::getUpperValueSingleton() {
    return m_upVal;
}

void MultiplicityElement::init() {
    m_lowVal.subsets(*m_ownedElements);
    m_lowVal.m_signature = &MultiplicityElement::getLowerValueSingleton;
    m_lowVal.m_addFunctors.insert(new AddLowerFunctor(this));
    m_lowVal.m_removeFunctors.insert(new RemoveLowerFunctor(this));
    m_upVal.subsets(*m_ownedElements);
    m_upVal.m_signature = &MultiplicityElement::getUpperValueSingleton;
    m_upVal.m_addFunctors.insert(new AddUpperFunctor(this));
    m_upVal.m_removeFunctors.insert(new RemoveUpperFunctor(this));
}

void MultiplicityElement::copy(const MultiplicityElement& rhs) {
    m_lowVal = rhs.m_lowVal;
    m_upVal = rhs.m_upVal;
    m_upper = rhs.m_upper;
    m_lower = rhs.m_lower;
    m_upSpecified = rhs.m_upSpecified;
    m_lowSpecified = rhs.m_lowSpecified;
    m_multiplicityIsSpecified = rhs.m_multiplicityIsSpecified;
}

MultiplicityElement::MultiplicityElement() : Element(ElementType::MULTIPLICITY_ELEMENT) {
    init();
}

MultiplicityElement::MultiplicityElement(const MultiplicityElement& rhs) : Element(ElementType::MULTIPLICITY_ELEMENT) {
    // abstract
}

MultiplicityElement::~MultiplicityElement() {
    
}

int MultiplicityElement::getLower() {
    if (m_lowSpecified) {
        return m_lower;
    } else {
        throw multiplicityNotSpecifiedException;
    }
}

void MultiplicityElement::setLower(const int low) {
    m_lower = low;
    if (!m_lowSpecified) {
        m_lowSpecified = true;
        updateCopiesScalar(m_lowSpecified, &MultiplicityElement::m_lowSpecified);
    }
    if (m_upSpecified) {
        m_multiplicityIsSpecified = true;
        updateCopiesScalar(true, &MultiplicityElement::m_multiplicityIsSpecified);
    }
    updateCopiesScalar(low, &MultiplicityElement::m_lower);
}

int MultiplicityElement::getUpper() {
    if (m_upSpecified) {
        return m_upper;
    } else {
        throw multiplicityNotSpecifiedException;
    }
}

void MultiplicityElement::setUpper(const int up) {
    m_upper = up;
    if (!m_upSpecified) {
        m_upSpecified = true;
        updateCopiesScalar(true, &MultiplicityElement::m_upSpecified);
    }
    if (m_lowSpecified) {
        m_multiplicityIsSpecified = true;
        updateCopiesScalar(true, &MultiplicityElement::m_multiplicityIsSpecified);
    }
    updateCopiesScalar(up, &MultiplicityElement::m_upper);
}

bool MultiplicityElement::multiplicitySpecified() {
    return m_multiplicityIsSpecified;
}

ValueSpecification* MultiplicityElement::getLowerValue() {
    return m_lowVal.get();
}

ValueSpecification& MultiplicityElement::getLowerValueRef() {
    return m_lowVal.getRef();
}

bool MultiplicityElement::hasLowerValue() const {
    return m_lowVal.has();
}

void MultiplicityElement::setLowerValue(ValueSpecification* val) {
    m_lowVal.set(val);
}

void MultiplicityElement::setLowerValue(ValueSpecification& val) {
    m_lowVal.set(val);
}

ValueSpecification* MultiplicityElement::getUpperValue() {
    return m_upVal.get();
}

ValueSpecification& MultiplicityElement::getUpperValueRef() {
    return m_upVal.getRef();
}

bool MultiplicityElement::hasUpperValue() const {
    return m_upVal.has();
}

void MultiplicityElement::setUpperValue(ValueSpecification* val) {
    m_upVal.set(val);
}

void MultiplicityElement::setUpperValue(ValueSpecification& val) {
    m_upVal.set(val);
}

bool MultiplicityElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MULTIPLICITY_ELEMENT;
    }

    return ret;
}