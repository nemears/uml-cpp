#include "uml/multiplicityElement.h"
#include "uml/literalInt.h"
#include "uml/umlPtr.h"
#include "uml/package.h"
#include "uml/behavior.h"
#include "uml/stereotype.h"
#include "uml/association.h"
#include "uml/dataType.h"
#include "uml/interface.h"
#include "uml/deployment.h"

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

MultiplicityElement::MultiplicityElement() : Element(ElementType::MULTIPLICITY_ELEMENT) {
    init();
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
    }
    if (m_upSpecified) {
        m_multiplicityIsSpecified = true;
    }
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
    }
    if (m_lowSpecified) {
        m_multiplicityIsSpecified = true;
    }
}

bool MultiplicityElement::multiplicitySpecified() {
    return m_multiplicityIsSpecified;
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

bool MultiplicityElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MULTIPLICITY_ELEMENT;
    }

    return ret;
}