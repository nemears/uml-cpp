#include "uml/association.h"
#include "uml/property.h"
#include "uml/umlPtr.h"
#include "uml/package.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/stereotype.h"
#include "uml/deployment.h"

using namespace UML;

void Association::AddEndTypeFunctor::operator()(Element& el) const {
    if (el.as<Property>().getType()) {
        if (el.as<Property>().m_type.loaded(el.as<Property>().m_type.ids().front())) {
            m_el.as<Association>().getEndType().add(*el.as<Property>().getType());
            el.as<Property>().getType()->setReference(&m_el);
        }
    }
}

void Association::RemoveEndTypeFunctor::operator()(Element& el) const {
    if (el.as<Property>().getType()) {
        if (m_el.as<Association>().getEndType().contains(el.as<Property>().getType().id())) {
            m_el.as<Association>().getEndType().remove(el.as<Property>().getType().id());
            el.as<Property>().getType()->removeReference(m_el.getID());
        }
    }
}

void Association::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    Relationship::referencingReleased(id);
}

void Association::referenceReindexed(ID oldID, ID newID) {
    Classifier::referenceReindexed(oldID, newID);
    Relationship::referenceReindexed(oldID, newID);
}

void Association::reindexName(std::string oldName, std::string newName) {
    Classifier::reindexName(oldName, newName);
    Relationship::reindexName(oldName, newName);
}

void Association::restoreReferences() {
    Classifier::restoreReferences();
    Relationship::restoreReferences();
    for (auto& prop : m_memberEnds) {
        if (prop.getType()) {
            m_endType.add(prop.getType().id());
        }
    }
}

void Association::restoreReference(Element* el) {
    Element::restoreReference(el);
    if (m_endType.count(el->getID())) {
        el->setReference(this);
    }
}

void Association::referenceErased(ID id) {
    Classifier::referenceErased(id);
    Relationship::referenceErased(id);
}

Set<Property, Association>& Association::getMemberEndsSet() {
    return m_memberEnds;
}

Set<Property, Association>& Association::getOwnedEndsSet() {
    return m_ownedEnds;
}

void Association::init() {
    m_memberEnds.subsets(m_members);
    m_memberEnds.opposite(&Property::getAssociationSingleton);
    m_memberEnds.m_signature = &Association::getMemberEndsSet;
    m_memberEnds.m_addFunctors.insert(new AddEndTypeFunctor(this));
    m_memberEnds.m_removeFunctors.insert(new RemoveEndTypeFunctor(this));
    m_ownedEnds.subsets(m_memberEnds);
    m_ownedEnds.subsets(m_ownedMembers);
    m_ownedEnds.subsets(m_features);
    m_ownedEnds.opposite(&Property::getOwningAssociationSingleton);
    m_ownedEnds.m_signature = &Association::getOwnedEndsSet;
    m_navigableOwnedEnds.subsets(m_ownedEnds);
    m_navigableOwnedEnds.m_signature = &Association::getNavigableOwnedEnds;
    m_endType.subsets(m_relatedElements);
    m_endType.m_signature = &Association::getEndType;
}

Association::Association() : Element(ElementType::ASSOCIATION) {
    init();
}

Association::~Association() {
    mountAndRelease();
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

Set<Type, Association>& Association::getEndType() {
    return m_endType;
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