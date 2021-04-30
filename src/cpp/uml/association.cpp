#include "uml/association.h"

using namespace UML;

void Association::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    Relationship::reindexID(oldID, newID);
    Classifier::reindexID(oldID, newID);
}

void Association::reindexName(std::string oldName, std::string newName) {
    Classifier::reindexName(oldName, newName);
}

void Association::AddMemberEndFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getAssociation() != m_el) {
        dynamic_cast<Property&>(el).setAssociation(dynamic_cast<Association*>(m_el));
    }

    if (!dynamic_cast<Association*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getMembers().add(dynamic_cast<Property&>(el));
    }
}

void Association::RemoveMemberEndFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getAssociation() == m_el) {
        dynamic_cast<Property&>(el).setAssociation(0);
    }

    if (dynamic_cast<Association*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getMembers().remove(dynamic_cast<Property&>(el));
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

Association::Association() {
    m_memberEnds = new Sequence<Property>;
    m_memberEnds->addProcedures.push_back(new AddMemberEndFunctor(this));
    m_memberEnds->removeProcedures.push_back(new RemoveMemberEndFunctor(this));
    m_ownedEnds = new Sequence<Property>;
    m_ownedEnds->addProcedures.push_back(new AddOwnedEndFunctor(this));
    m_ownedEnds->removeProcedures.push_back(new RemoveOwnedEndFunctor(this));
    m_navigableOwnedEnds = new Sequence<Property>;
    m_navigableOwnedEnds->addProcedures.push_back(new AddNavigableOwnedEndFunctor(this));
    m_navigableOwnedEnds->removeProcedures.push_back(new RemoveNavigableOwnedEndFunctor(this));
}

Association::~Association() {
    delete m_memberEnds;
    delete m_ownedEnds;
    delete m_navigableOwnedEnds;
}

Sequence<Property>& Association::getMemberEnds() {
    return *m_memberEnds;
}

Sequence<Property>& Association::getOwnedEnds() {
    return *m_ownedEnds;
}

Sequence<Property>& Association::getNavigableOwnedEnds() {
    return *m_navigableOwnedEnds;
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