#include "uml/enumerationLiteral.h"
#include "uml/enumeration.h"
#include "uml/uml-stable.h"

using namespace UML;

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
    if (!m_copiedElementFlag) {
        delete &rhs;
    }
}

EnumerationLiteral::~EnumerationLiteral() {
    mountAndRelease();
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

void EnumerationLiteral::setEnumeration(ID id) {
    m_enumeration.set(id);
}

bool EnumerationLiteral::isSubClassOf(ElementType eType) const {
    bool ret = InstanceSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION_LITERAL;
    }

    return ret;
}