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
        if (el.as<Property>().getType().loaded() && !m_el.as<Association>().getEndTypes().contains(el.as<Property>().getType().id())) {
            m_el.as<Association>().getEndTypes().add(*el.as<Property>().getType());
            el.as<Property>().getType()->setReference(&m_el);
        }
    }
}

void Association::RemoveEndTypeFunctor::operator()(Element& el) const {
    if (el.as<Property>().getType()) {
        if (m_el.as<Association>().getEndTypes().contains(el.as<Property>().getType().id())) {
            m_el.as<Association>().getEndTypes().remove(el.as<Property>().getType().id());
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

void Association::reindexName(ID id, std::string newName) {
    Classifier::reindexName(id, newName);
    Relationship::reindexName(id, newName);
}

void Association::restoreReferences() {
    Classifier::restoreReferences();
    Relationship::restoreReferences();
    for (auto& prop : m_memberEnds) {
        if (prop.getType()) {
            m_endTypes.add(prop.getType().id());
        }
    }
}

void Association::restoreReference(Element* el) {
    Element::restoreReference(el);
    if (m_endTypes.count(el->getID())) {
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
    m_memberEnds.m_addFunctors.insert(new AddEndTypeFunctor(this));
    m_memberEnds.m_removeFunctors.insert(new RemoveEndTypeFunctor(this));
    m_ownedEnds.subsets(m_memberEnds);
    m_ownedEnds.subsets(m_ownedMembers);
    m_ownedEnds.subsets(m_features);
    m_ownedEnds.opposite(&Property::getOwningAssociationSingleton);
    m_navigableOwnedEnds.subsets(m_ownedEnds);
    m_endTypes.subsets(m_relatedElements);
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