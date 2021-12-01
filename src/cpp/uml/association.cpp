#include "uml/association.h"
#include "uml/property.h"
#include "uml/uml-stable.h"

using namespace UML;

void Association::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    Relationship::referencingReleased(id);
    m_memberEnds.release(id);
    m_ownedEnds.release(id);
    m_navigableOwnedEnds.release(id);
    m_endType.release(id);
}

void Association::referenceReindexed(ID oldID, ID newID) {
    Classifier::referenceReindexed(oldID, newID);
    Relationship::referenceReindexed(oldID, newID);
    m_memberEnds.reindex(oldID, newID);
    m_ownedEnds.reindex(oldID, newID);
    m_navigableOwnedEnds.reindex(oldID, newID);
    m_endType.reindex(oldID, newID);
}

void Association::restoreReferences() {
    Classifier::restoreReferences();
    Relationship::restoreReferences();
    // m_memberEnds.restoreReferences();
    // m_ownedEnds.restoreReferences();
    // m_navigableOwnedEnds.restoreReferences();
    // for (auto& end : m_memberEnds) {
    //     if (end.hasType()) {
    //         if (!m_endType.count(end.getTypeID())) {
    //             if (m_manager->loaded(end.getTypeID())) {
    //                 m_endType.add(end.getTypeRef());
    //             } else {
    //                 m_endType.addByID(end.getTypeID());
    //             }
    //         }
    //     }
    // }
    // m_endType.restoreReferences();
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
    m_memberEnds.eraseElement(id);
    m_ownedEnds.eraseElement(id);
    m_navigableOwnedEnds.eraseElement(id);
    m_endType.eraseElement(id);
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
    m_ownedEnds.subsets(m_ownedMembers);
    m_ownedEnds.subsets(m_features);
    m_ownedEnds.subsets(m_memberEnds);
    m_ownedEnds.opposite(&Property::getOwningAssociationSingleton);
    m_ownedEnds.m_signature = &Association::getOwnedEndsSet;
    m_navigableOwnedEnds.subsets(m_ownedEnds);
    m_navigableOwnedEnds.m_signature = &Association::getNavigableOwnedEnds;
    m_endType.subsets(m_relatedElements);
    m_endType.m_signature = &Association::getEndType;
}

void Association::copy(const Association& rhs) {
    m_memberEnds = rhs.m_memberEnds;
    m_ownedEnds = rhs.m_ownedEnds;
    m_navigableOwnedEnds = rhs.m_navigableOwnedEnds;
    m_endType = rhs.m_endType;
}

Association::Association() : Element(ElementType::ASSOCIATION) {
    init();
}

Association::Association(const Association& rhs) : Element(rhs, ElementType::ASSOCIATION) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    PackageableElement::copy(rhs);
    Classifier::copy(rhs);
    Relationship::copy(rhs);
    copy(rhs);
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