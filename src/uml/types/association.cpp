#include "uml/uml-stable.h"

using namespace UML;

void Association::MemberEndPolicy::elementAdded(Property& el, Association& me) {
    if (el.getType()) {
        if (el.getType().loaded() && !me.getEndTypes().contains(el.getType().id())) {
            me.m_endTypes.innerAdd(el.getType());
        }
    }
}

void Association::MemberEndPolicy::elementRemoved(Property& el, Association& me) {
    if (el.getType()) {
        if (me.getEndTypes().contains(el.getType().id())) {
            me.m_endTypes.innerRemove(el.getType());
        }
    }
}

void Association::restoreReferences() {
    Classifier::restoreReferences();
    Relationship::restoreReferences();
    for (auto& prop : m_memberEnds) {
        if (prop.getType() && !m_endTypes.contains(prop.getType().id())) {
            m_endTypes.add(prop.getType());
        }
    }
}

void Association::referenceErased(ID id) {
    Classifier::referenceErased(id);
    Relationship::referenceErased(id);
}

Association::Association() : Element(ElementType::ASSOCIATION) {
    m_memberEnds.subsets(m_members);
    m_memberEnds.opposite(&Property::getAssociationSingleton);
    m_ownedEnds.subsets(m_memberEnds);
    m_ownedEnds.subsets(m_ownedMembers);
    m_ownedEnds.subsets(m_features);
    m_ownedEnds.opposite(&Property::getOwningAssociationSingleton);
    m_navigableOwnedEnds.subsets(m_ownedEnds);
    m_endTypes.subsets(m_relatedElements);
}

Association::~Association() {
    
}

IndexableOrderedSet<Property, Association, Association::MemberEndPolicy>& Association::getMemberEnds() {
    return m_memberEnds;
}

IndexableOrderedSet<Property, Association>& Association::getOwnedEnds() {
    return m_ownedEnds;
}

IndexableSet<Property, Association>& Association::getNavigableOwnedEnds() {
    return m_navigableOwnedEnds;
}

IndexableSet<Type, Association>& Association::getEndTypes() {
    return m_endTypes;
}

bool Association::is(ElementType eType) const {
    bool ret = Relationship::is(eType);

    if (!ret) {
        ret = Classifier::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::ASSOCIATION;
    }

    return ret;
}
