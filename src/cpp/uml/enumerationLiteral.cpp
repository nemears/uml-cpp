#include "uml/enumerationLiteral.h"
#include "uml/enumeration.h"

using namespace UML;

void EnumerationLiteral::RemoveEnumerationProcedure::operator()(Enumeration* el) const {
    if (el->getOwnedLiteral().count(m_me->getID())) {
        el->getOwnedLiteral().remove(*m_me);
    }
}

void EnumerationLiteral::AddEnumerationProcedure::operator()(Enumeration* el) const {
    if (!el->getOwnedLiteral().count(m_me->getID())) {
        el->getOwnedLiteral().add(*m_me);
    }
}

void EnumerationLiteral::referencingReleased(ID id) {
    InstanceSpecification::referencingReleased(id);
    if (m_enumeration.id() == id) {
        m_enumeration.release();
    }
}

void EnumerationLiteral::referenceReindexed(ID oldID, ID newID) {
    InstanceSpecification::referenceReindexed(oldID, newID);
    if (m_enumeration.id() == oldID) {
        m_enumeration.reindex(oldID, newID);
    }
}

EnumerationLiteral::EnumerationLiteral() {
    m_enumeration.m_signature = &EnumerationLiteral::m_enumeration;
    m_enumeration.m_addProcedures.push_back(new AddEnumerationProcedure(this));
    m_enumeration.m_removeProcedures.push_back(new RemoveEnumerationProcedure(this));
}

EnumerationLiteral::EnumerationLiteral(const EnumerationLiteral& rhs) {
    m_enumeration = rhs.m_enumeration;
    m_enumeration.m_me = this;
    m_enumeration.m_removeProcedures.clear();
    m_enumeration.m_addProcedures.clear();
    m_enumeration.m_addProcedures.push_back(new AddEnumerationProcedure(this));
    m_enumeration.m_removeProcedures.push_back(new RemoveEnumerationProcedure(this));
}

Enumeration* EnumerationLiteral::getEnumeration() {
    return m_enumeration.get();
}

Enumeration& EnumerationLiteral::getEnumerationRef() {
    return m_enumeration.getRef();
}

bool EnumerationLiteral::hasEnumeration() const {
    return m_enumeration.has();
}

void EnumerationLiteral::setEnumeration(Enumeration* enumeration) {
    m_enumeration.set(enumeration);
}

void EnumerationLiteral::setEnumeration(Enumeration* enumeration) {
    m_enumeration.set(enumeration);
}

ElementType EnumerationLiteral::getElementType() const {
    return ElementType::ENUMERATION_LITERAL;
}

bool EnumerationLiteral::isSubClassOf(ElementType eType) const {
    bool ret = InstanceSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION_LITERAL;
    }

    return ret;
}