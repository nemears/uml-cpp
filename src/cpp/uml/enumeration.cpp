#include "uml/enumeration.h"
#include "uml/enumerationLiteral.h"

using namespace UML;

void Enumeration::AddOwnedLiteralFunctor::operator()(EnumerationLiteral& el) const {
    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    if (el.getEnumeration() != m_el) {
        el.setEnumeration(m_el);
    }
}

void Enumeration::RemoveOwnedLiteralFunctor::operator()(EnumerationLiteral& el) const {
    if (m_el->getMembers().count(el.getID())) {
        m_el->getMembers().remove(el);
    }

    if (el.getEnumeration() == m_el) {
        el.setEnumeration(0);
    }
}

Enumeration::Enumeration() {
    m_ownedLiteral.addProcedures.push_back(new AddOwnedLiteralFunctor(this));
    m_ownedLiteral.removeProcedures.push_back(new RemoveOwnedLiteralFunctor(this));
}

Enumeration::Enumeration(const Enumeration& enumeration) : DataType(enumeration) , PackageableElement(enumeration), NamedElement(enumeration),
                                                           Element(enumeration) {
    m_ownedLiteral = enumeration.m_ownedLiteral;
    m_ownedLiteral.addProcedures.clear();
    m_ownedLiteral.addProcedures.push_back(new AddOwnedLiteralFunctor(this));
    m_ownedLiteral.removeProcedures.clear();
    m_ownedLiteral.removeProcedures.push_back(new RemoveOwnedLiteralFunctor(this));
}

Sequence<EnumerationLiteral>& Enumeration::getOwnedLiteral() {
    return m_ownedLiteral;
}

ElementType Enumeration::getElementType() const {
    return ElementType::ENUMERATION;
}

bool Enumeration::isSubClassOf(ElementType eType) const {
    bool ret = DataType::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION;
    }

    return ret;
}