#include "uml/enumerationLiteral.h"
#include "uml/enumeration.h"
#include "uml/uml-stable.h"

using namespace UML;

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
    // m_enumeration.restoreReference();
}

void EnumerationLiteral::referenceErased(ID id) {
    InstanceSpecification::referenceErased(id);
    m_enumeration.eraseElement(id);
}

Set<Enumeration, EnumerationLiteral>& EnumerationLiteral::getEnumerationSingleton() {
    return m_enumeration;
}

void EnumerationLiteral::init() {
    m_enumeration.subsets(m_namespace);
    m_enumeration.opposite(&Enumeration::getOwnedLiteralsSet);
    m_enumeration.m_signature = &EnumerationLiteral::getEnumerationSingleton;
}

void EnumerationLiteral::copy(const EnumerationLiteral& rhs) {
    m_enumeration = rhs.m_enumeration;
}

EnumerationLiteral::EnumerationLiteral() : Element(ElementType::ENUMERATION_LITERAL) {
    init();
}

EnumerationLiteral::EnumerationLiteral(const EnumerationLiteral& rhs) : Element(rhs, ElementType::ENUMERATION_LITERAL) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    PackageableElement::copy(rhs);
    InstanceSpecification::copy(rhs);
    copy(rhs);
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