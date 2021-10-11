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
    updateCopiedSequenceAddedTo(el, &Enumeration::getOwnedLiterals);
}

void Enumeration::RemoveOwnedLiteralFunctor::operator()(EnumerationLiteral& el) const {
    if (m_el->getMembers().count(el.getID())) {
        m_el->getMembers().remove(el);
    }

    if (el.getEnumeration() == m_el) {
        el.setEnumeration(0);
    }
    updateCopiedSequenceRemovedFrom(el, &Enumeration::getOwnedLiterals);
}

void Enumeration::setManager(UmlManager* manager) {
    DataType::setManager(manager);
    m_ownedLiterals.m_manager = manager;
}

void Enumeration::referencingReleased(ID id) {
    DataType::referencingReleased(id);
    m_ownedLiterals.elementReleased(id, &Enumeration::getOwnedLiterals);
}

void Enumeration::referenceReindexed(ID oldID, ID newID) {
    DataType::referenceReindexed(oldID, newID);
    if (m_ownedLiterals.count(oldID)) {
        m_ownedLiterals.reindex(oldID, newID, &Enumeration::getOwnedLiterals);
    }
}

Enumeration::Enumeration() {
    m_ownedLiterals.addProcedures.push_back(new AddOwnedLiteralFunctor(this));
    m_ownedLiterals.removeProcedures.push_back(new RemoveOwnedLiteralFunctor(this));
}

Enumeration::Enumeration(const Enumeration& enumeration) : DataType(enumeration) , PackageableElement(enumeration), NamedElement(enumeration),
                                                           Element(enumeration) {
    m_ownedLiterals = enumeration.m_ownedLiterals;
    m_ownedLiterals.m_el = this;
    m_ownedLiterals.addProcedures.clear();
    m_ownedLiterals.addProcedures.push_back(new AddOwnedLiteralFunctor(this));
    m_ownedLiterals.removeProcedures.clear();
    m_ownedLiterals.removeProcedures.push_back(new RemoveOwnedLiteralFunctor(this));
}

Sequence<EnumerationLiteral>& Enumeration::getOwnedLiterals() {
    return m_ownedLiterals;
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