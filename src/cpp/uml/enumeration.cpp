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
    updateCopiedSequenceAddedTo(el, &Enumeration::getOwnedLiteral);
}

void Enumeration::RemoveOwnedLiteralFunctor::operator()(EnumerationLiteral& el) const {
    if (m_el->getMembers().count(el.getID())) {
        m_el->getMembers().remove(el);
    }

    if (el.getEnumeration() == m_el) {
        el.setEnumeration(0);
    }
    updateCopiedSequenceRemovedFrom(el, &Enumeration::getOwnedLiteral);
}

void Enumeration::setManager(UmlManager* manager) {
    DataType::setManager(manager);
    m_ownedLiteral.m_manager = manager;
}

void Enumeration::referencingReleased(ID id) {
    DataType::referencingReleased(id);
    m_ownedLiteral.elementReleased(id, &Enumeration::getOwnedLiteral);
}

void Enumeration::referenceReindexed(ID oldID, ID newID) {
    DataType::referenceReindexed(oldID, newID);
    if (m_ownedLiteral.count(oldID)) {
        m_ownedLiteral.reindex(oldID, newID, &Enumeration::getOwnedLiteral);
    }
}

Enumeration::Enumeration() {
    m_ownedLiteral.addProcedures.push_back(new AddOwnedLiteralFunctor(this));
    m_ownedLiteral.removeProcedures.push_back(new RemoveOwnedLiteralFunctor(this));
}

Enumeration::Enumeration(const Enumeration& enumeration) : DataType(enumeration) , PackageableElement(enumeration), NamedElement(enumeration),
                                                           Element(enumeration) {
    m_ownedLiteral = enumeration.m_ownedLiteral;
    m_ownedLiteral.m_el = this;
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