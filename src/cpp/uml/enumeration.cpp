#include "uml/enumeration.h"
#include "uml/enumerationLiteral.h"
#include "uml/uml-stable.h"

using namespace UML;

Set<EnumerationLiteral, Enumeration>& Enumeration::getOwnedLiteralsSet() {
    return m_ownedLiterals;
}

void Enumeration::init() {
    m_ownedLiterals.subsets(m_ownedMembers);
    m_ownedLiterals.opposite(&EnumerationLiteral::getEnumerationSingleton);
    m_ownedLiterals.m_signature = &Enumeration::getOwnedLiteralsSet;
}

void Enumeration::copy(const Enumeration& rhs) {
    m_ownedLiterals = rhs.m_ownedLiterals;
}

Enumeration::Enumeration() : Element(ElementType::ENUMERATION) {
    init();
}

Enumeration::Enumeration(const Enumeration& rhs) : Element(rhs, ElementType::ENUMERATION) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    PackageableElement::copy(rhs);
    Classifier::copy(rhs);
    DataType::copy(rhs);
    copy(rhs);
}

OrderedSet<EnumerationLiteral, Enumeration>& Enumeration::getOwnedLiterals() {
    return m_ownedLiterals;
}

bool Enumeration::isSubClassOf(ElementType eType) const {
    bool ret = DataType::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION;
    }

    return ret;
}