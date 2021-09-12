#include "uml/association.h"
#include "uml/property.h"

using namespace UML;

void Association::reindexID(ID oldID, ID newID) {
    Relationship::reindexID(oldID, newID);
    Classifier::reindexID(oldID, newID);
}

void Association::reindexName(std::string oldName, std::string newName) {
    Classifier::reindexName(oldName, newName);
}

void Association::AddMemberEndFunctor::operator()(Property& el) const {
    if (el.getAssociation() != m_el) {
        el.setAssociation(m_el);
    }

    if (!m_el->getMembers().count(el.getID())) {
        m_el->getMembers().add(el);
    }

    if (el.getType()) {
        if (!m_el->getEndType().count(el.getType()->getID())) {
            m_el->getEndType().add(*el.getType());
        }
    }
}

void Association::RemoveMemberEndFunctor::operator()(Property& el) const {
    if (el.getAssociation() == m_el) {
        el.setAssociation(0);
    }

    if (m_el->getMembers().count(el.getID())) {
        m_el->getMembers().remove(el);
    }

    if (el.getType()) {
        if (m_el->getEndType().count(el.getType()->getID())) {
            m_el->getEndType().remove(*el.getType());
        }
    }
}

void Association::AddOwnedEndFunctor::operator()(Property& el) const {
    if (el.getOwningAssociation() != m_el) {
        el.setOwningAssociation(m_el);
    }

    if (!m_el->getMemberEnds().count(el.getID())) {
        m_el->getMemberEnds().add(el);
    }

    if (!m_el->getFeatures().count(el.getID())) {
        m_el->getFeatures().add(el);
    }

    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }
}

void Association::RemoveOwnedEndFunctor::operator()(Property& el) const {
    if (el.getOwningAssociation() == m_el) {
        el.setOwningAssociation(0);
    }

    if (m_el->getMemberEnds().count(el.getID())) {
        m_el->getMemberEnds().remove(el);
    }

    if (m_el->getFeatures().count(el.getID())) {
        m_el->getFeatures().remove(el);
    }

    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().remove(el);
    }
}

void Association::AddNavigableOwnedEndFunctor::operator()(Property& el) const {
    if (!m_el->getOwnedEnds().count(el.getID())) {
        m_el->getOwnedEnds().add(el);
    }
}

void Association::RemoveNavigableOwnedEndFunctor::operator()(Property& el) const {
    if (dynamic_cast<Association*>(m_el)->getOwnedEnds().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getOwnedEnds().remove(dynamic_cast<Property&>(el));
    }
}

void Association::setManager(UmlManager* manager) {
    Classifier::setManager(manager);
    m_relatedElements.m_manager = manager;
    m_memberEnds.m_manager = manager;
    m_ownedEnds.m_manager = manager;
    m_navigableOwnedEnds.m_manager = manager;
    m_endType.m_manager = manager;
}

void Association::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    Relationship::referenceReindexed(oldID, newID);
    /** TODO: finish**/
}

Association::Association() {
    m_memberEnds.addProcedures.push_back(new AddMemberEndFunctor(this));
    m_memberEnds.removeProcedures.push_back(new RemoveMemberEndFunctor(this));
    m_ownedEnds.addProcedures.push_back(new AddOwnedEndFunctor(this));
    m_ownedEnds.removeProcedures.push_back(new RemoveOwnedEndFunctor(this));
    m_navigableOwnedEnds.addProcedures.push_back(new AddNavigableOwnedEndFunctor(this));
    m_navigableOwnedEnds.removeProcedures.push_back(new RemoveNavigableOwnedEndFunctor(this));
}

Association::~Association() {
    // nothing
}

Sequence<Property>& Association::getMemberEnds() {
    return m_memberEnds;
}

Sequence<Property>& Association::getOwnedEnds() {
    return m_ownedEnds;
}

Sequence<Property>& Association::getNavigableOwnedEnds() {
    return m_navigableOwnedEnds;
}

Sequence<Type>& Association::getEndType() {
    return m_endType;
}

ElementType Association::getElementType() const {
    return ElementType::ASSOCIATION;
}

bool Association::isSubClassOf(ElementType eType) const {
    bool ret = Relationship::isSubClassOf(eType);

    if (!ret) {
        ret = Classifier::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::ASSOCIATION;
    }

    return ret;
}