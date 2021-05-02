#include "uml/enumeration.h"

using namespace UML;

void Enumeration::AddOwnedLiteralFunctor::operator()(Element& el) const {
    if (!dynamic_cast<Enumeration*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<Enumeration*>(m_el)->getOwnedMembers().add(dynamic_cast<EnumerationLiteral&>(el));
    }

    if (dynamic_cast<EnumerationLiteral&>(el).getEnumeration() != m_el) {
        dynamic_cast<EnumerationLiteral&>(el).setEnumeration(dynamic_cast<Enumeration*>(m_el));
    }
}

void Enumeration::RemoveOwnedLiteralFunctor::operator()(Element& el) const {
    if (dynamic_cast<Enumeration*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<Enumeration*>(m_el)->getMembers().remove(dynamic_cast<EnumerationLiteral&>(el));
    }

    if (dynamic_cast<EnumerationLiteral&>(el).getEnumeration() == m_el) {
        dynamic_cast<EnumerationLiteral&>(el).setEnumeration(0);
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

bool Enumeration::isSubClassOf(ElementType eType) {
    bool ret = DataType::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION;
    }

    return ret;
}