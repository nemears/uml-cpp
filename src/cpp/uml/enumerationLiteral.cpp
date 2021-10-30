#include "uml/enumerationLiteral.h"
#include "uml/enumeration.h"

using namespace UML;

void EnumerationLiteral::RemoveEnumerationProcedure::operator()(Enumeration* el) const {
    if (el->getOwnedLiterals().count(m_me->getID())) {
        el->getOwnedLiterals().remove(*m_me);
    }
    if (m_me->getNamespaceID() == el->getID()) {
        m_me->setNamespace(0);
    }
}

void EnumerationLiteral::AddEnumerationProcedure::operator()(Enumeration* el) const {
    if (!el->getOwnedLiterals().count(m_me->getID())) {
        el->getOwnedLiterals().add(*m_me);
    }
    if (m_me->getNamespaceID() != el->getID()) {
        m_me->setNamespace(el);
    }
}

void EnumerationLiteral::AddEnumerationProcedure::operator()(ID id) const {
    if (m_me->getNamespaceID() != id) {
        m_me->m_namespace.setByID(id);
    }
}

void EnumerationLiteral::referencingReleased(ID id) {
    InstanceSpecification::referencingReleased(id);
    m_enumeration.release(id);
}

void EnumerationLiteral::referenceReindexed(ID oldID, ID newID) {
    InstanceSpecification::referenceReindexed(oldID, newID);
    m_enumeration.reindex(oldID, newID);
}

void EnumerationLiteral::restoreReferences() {
    InstanceSpecification::restoreReferences();
    m_enumeration.restoreReference();
}

void EnumerationLiteral::referenceErased(ID id) {
    InstanceSpecification::referenceErased(id);
    m_enumeration.elementErased(id);
}

EnumerationLiteral::EnumerationLiteral() : Element(ElementType::ENUMERATION_LITERAL) {
    m_enumeration.m_signature = &EnumerationLiteral::m_enumeration;
    m_enumeration.m_addProcedures.push_back(new AddEnumerationProcedure(this));
    m_enumeration.m_removeProcedures.push_back(new RemoveEnumerationProcedure(this));
}

EnumerationLiteral::EnumerationLiteral(const EnumerationLiteral& rhs) : Element(rhs, ElementType::ENUMERATION_LITERAL) {
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

ID EnumerationLiteral::getEnumerationID() const {
    return m_enumeration.id();
}

bool EnumerationLiteral::hasEnumeration() const {
    return m_enumeration.has();
}

void EnumerationLiteral::setEnumeration(Enumeration* enumeration) {
    m_enumeration.set(enumeration);
}

void EnumerationLiteral::setEnumeration(Enumeration& enumeration) {
    m_enumeration.set(enumeration);
}

bool EnumerationLiteral::isSubClassOf(ElementType eType) const {
    bool ret = InstanceSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION_LITERAL;
    }

    return ret;
}