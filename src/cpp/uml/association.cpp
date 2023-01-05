#include "uml/association.h"
#include "uml/property.h"
#include "uml/umlPtr.h"
#include "uml/package.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/stereotype.h"
#include "uml/deployment.h"

using namespace UML;

void Association::AddMemberEndPolicy::apply(Property& el, Association& me) {
    if (el.getType()) {
        if (el.getType().loaded() && !me.getEndTypes().contains(el.getType().id())) {
            [[maybe_unused]] SetLock typeLock = me.lockEl(*el.getType());
            me.m_endTypes.innerAdd(*el.getType());
            el.getType()->setReference(&me);
            me.setReference(el.getType().ptr());
        }
    }
}

void Association::RemoveMemberEndPolicy::apply(Property& el, Association& me) {
    if (el.getType()) {
        if (me.getEndTypes().contains(el.getType().id())) {
            [[maybe_unused]] SetLock typeLock = me.lockEl(*el.getType());
            me.m_endTypes.innerRemove(el.getType().id());
            el.getType()->removeReference(me.getID());
            me.removeReference(el.getType().id());
        }
    }
}

void Association::referenceReindexed(ID newID) {
    Classifier::referenceReindexed(newID);
    Relationship::referenceReindexed(newID);
}

void Association::restoreReferences() {
    Classifier::restoreReferences();
    Relationship::restoreReferences();
    for (auto& prop : m_memberEnds) {
        if (prop.getType() && !m_endTypes.contains(prop.getType().id())) {
            m_endTypes.add(prop.getType().id());
        }
    }
}

void Association::restoreReference(Element* el) {
    Element::restoreReference(el);
    if (m_endTypes.contains(el->getID())) {
        el->setReference(this);
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

OrderedSet<Property, Association>& Association::getMemberEnds() {
    return m_memberEnds;
}

OrderedSet<Property, Association>& Association::getOwnedEnds() {
    return m_ownedEnds;
}

Set<Property, Association>& Association::getNavigableOwnedEnds() {
    return m_navigableOwnedEnds;
}

Set<Type, Association>& Association::getEndTypes() {
    return m_endTypes;
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