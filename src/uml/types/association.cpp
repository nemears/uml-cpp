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

Association::Association(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager),
    Relationship(elementType, manager)
{
    m_memberEnds.subsets(m_members);
    m_memberEnds.opposite(&Property::getAssociationSingleton);
    m_ownedEnds.subsets(m_memberEnds);
    m_ownedEnds.subsets(m_ownedMembers);
    m_ownedEnds.subsets(m_features);
    m_ownedEnds.opposite(&Property::getOwningAssociationSingleton);
    m_navigableOwnedEnds.subsets(m_ownedEnds);
    m_endTypes.subsets(m_relatedElements);
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

ReadOnlyIndexableSet<Type, Association>& Association::getEndTypes() {
    return m_endTypes;
}
