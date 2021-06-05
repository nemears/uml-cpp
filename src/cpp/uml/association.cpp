#include "uml/association.h"

using namespace UML;

void Association::reindexID(ID oldID, ID newID) {
    Relationship::reindexID(oldID, newID);
    Classifier::reindexID(oldID, newID);
}

// void Association::reindexName(std::string oldName, std::string newName) {
//     Classifier::reindexName(oldName, newName);
// }

void Association::AddMemberEndFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getAssociation() != m_el) {
        dynamic_cast<Property&>(el).setAssociation(dynamic_cast<Association*>(m_el));
    }

    if (!dynamic_cast<Association*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getMembers().add(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<Property&>(el).getType()) {
        if (!dynamic_cast<Association*>(m_el)->getEndType().count(dynamic_cast<Property&>(el).getType()->getID())) {
            dynamic_cast<Association*>(m_el)->getEndType().add(*dynamic_cast<Property&>(el).getType());
        }
    }
}

void Association::RemoveMemberEndFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getAssociation() == m_el) {
        dynamic_cast<Property&>(el).setAssociation(0);
    }

    if (dynamic_cast<Association*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getMembers().remove(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<Property&>(el).getType()) {
        if (dynamic_cast<Association*>(m_el)->getEndType().count(dynamic_cast<Property&>(el).getType()->getID())) {
            dynamic_cast<Association*>(m_el)->getEndType().remove(*dynamic_cast<Property&>(el).getType());
        }
    }
}

void Association::AddOwnedEndFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getOwningAssociation() != m_el) {
        dynamic_cast<Property&>(el).setOwningAssociation(dynamic_cast<Association*>(m_el));
    }

    if (!dynamic_cast<Association*>(m_el)->getMemberEnds().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getMemberEnds().add(dynamic_cast<Property&>(el));
    }

    if (!dynamic_cast<Association*>(m_el)->getFeatures().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getFeatures().add(dynamic_cast<Property&>(el));
    }

    if (!dynamic_cast<Association*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getOwnedMembers().add(dynamic_cast<Property&>(el));
    }
}

void Association::RemoveOwnedEndFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getOwningAssociation() == m_el) {
        dynamic_cast<Property&>(el).setOwningAssociation(0);
    }

    if (dynamic_cast<Association*>(m_el)->getMemberEnds().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getMemberEnds().remove(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<Association*>(m_el)->getFeatures().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getFeatures().remove(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<Association*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getOwnedMembers().remove(dynamic_cast<Property&>(el));
    }
}

void Association::AddNavigableOwnedEndFunctor::operator()(Element& el) const {
    if (!dynamic_cast<Association*>(m_el)->getOwnedEnds().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getOwnedEnds().add(dynamic_cast<Property&>(el));
    }
}

void Association::RemoveNavigableOwnedEndFunctor::operator()(Element& el) const {
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

bool Association::isSubClassOf(ElementType eType) {
    bool ret = Relationship::isSubClassOf(eType);

    if (!ret) {
        ret = Classifier::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::ASSOCIATION;
    }

    return ret;
}