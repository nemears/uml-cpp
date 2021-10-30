#include "uml/multiplicityElement.h"
#include "uml/literalInt.h"
#include "uml/valueSpecification.h"

using namespace UML;

void MultiplicityElement::RemoveLowerValueProcedures::operator()(ValueSpecification* el) const {
    if (m_me->m_lowSpecified) {
        m_me->m_lower = -1;
        m_me->m_lowSpecified = false;
        m_me->m_multiplicityIsSpecified = false;
    }

    if (m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalRemove(*el);
    }
    m_me->updateCopiesScalar(-1, &MultiplicityElement::m_lower);
    m_me->updateCopiesScalar(false, &MultiplicityElement::m_lowSpecified);
    m_me->updateCopiesScalar(false, &MultiplicityElement::m_multiplicityIsSpecified);
}

void MultiplicityElement::AddLowerValueProcedures::operator()(ValueSpecification* el) const {
    if (el->isSubClassOf(ElementType::LITERAL_INT)) {
        if (dynamic_cast<LiteralInt*>(el)->getValue() >= 0) {
            m_me->setLower(dynamic_cast<LiteralInt*>(el)->getValue());
        }
    }
    else if (el->isSubClassOf(ElementType::EXPRESSION)) {
        // TODO evaluate expression
    }

    if (!m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalAdd(*el);
    }
}

void MultiplicityElement::RemoveUpperValueProcedures::operator()(ValueSpecification* el) const {
    if (m_me->m_upSpecified) {
        m_me->m_upper = -1;
        m_me->m_upSpecified = false;
        m_me->m_multiplicityIsSpecified = false;
    }

    if (m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalRemove(*el);
    }
}

void MultiplicityElement::AddUpperValueProcedures::operator()(ValueSpecification* el) const {
    if (el->isSubClassOf(ElementType::LITERAL_INT)) {
        if (dynamic_cast<LiteralInt*>(el)->getValue() >= 0) {
            m_me->setUpper(dynamic_cast<LiteralInt*>(el)->getValue());
        }
    }
    else if (el->isSubClassOf(ElementType::EXPRESSION)) {
        // TODO evaluate expression
    }

    if (!m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalAdd(*el);
    }
}

void MultiplicityElement::referencingReleased(ID id) {
    m_lowVal.release(id);
    m_upVal.release(id);
}

void MultiplicityElement::referenceReindexed(ID oldID, ID newID) {
    m_lowVal.reindex(oldID, newID);
    m_lowVal.reindex(oldID, newID);
}

void MultiplicityElement::restoreReferences() {
    m_lowVal.restoreReference();
    m_upVal.restoreReference();
}

void MultiplicityElement::referenceErased(ID id) {
    m_lowVal.elementErased(id);
    m_upVal.elementErased(id);
}

MultiplicityElement::MultiplicityElement() : Element(ElementType::MULTIPLICITY_ELEMENT) {
    m_lowVal.m_signature = &MultiplicityElement::m_lowVal;
    m_lowVal.m_removeProcedures.push_back(new RemoveLowerValueProcedures(this));
    m_lowVal.m_addProcedures.push_back(new AddLowerValueProcedures(this));
    m_upVal.m_signature = &MultiplicityElement::m_upVal;
    m_upVal.m_removeProcedures.push_back(new RemoveUpperValueProcedures(this));
    m_upVal.m_addProcedures.push_back(new AddUpperValueProcedures(this));
}

MultiplicityElement::MultiplicityElement(const MultiplicityElement& rhs) : Element(rhs, ElementType::MULTIPLICITY_ELEMENT) {
    m_lowVal = rhs.m_lowVal;
    m_lowVal.m_me = this;
    m_lowVal.m_removeProcedures.clear();
    m_lowVal.m_addProcedures.clear();
    m_lowVal.m_removeProcedures.push_back(new RemoveLowerValueProcedures(this));
    m_lowVal.m_addProcedures.push_back(new AddLowerValueProcedures(this));
    m_upVal = rhs.m_upVal;
    m_upVal.m_me = this;
    m_upVal.m_removeProcedures.clear();
    m_upVal.m_addProcedures.clear();
    m_upVal.m_removeProcedures.push_back(new RemoveUpperValueProcedures(this));
    m_upVal.m_addProcedures.push_back(new AddUpperValueProcedures(this));
    m_upper = rhs.m_upper;
    m_lower = rhs.m_lower;
    m_upSpecified = rhs.m_upSpecified;
    m_lowSpecified = rhs.m_lowSpecified;
    m_multiplicityIsSpecified = rhs.m_multiplicityIsSpecified;
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