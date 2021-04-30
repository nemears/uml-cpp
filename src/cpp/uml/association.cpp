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

Association::Association() {
    m_memberEnds = new Sequence<Property>;
    m_memberEnds->addProcedures.push_back(new AddMemberEndFunctor(this));
    m_ownedEnds = new Sequence<Property>;
    m_navigableOwnedEnds = new Sequence<Property>;
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

ElementType Association::getElementType() {
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