#include "uml/association.h"
#include "uml/property.h"

using namespace UML;

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
    updateCopiedSequenceAddedTo(el, &Association::getMemberEnds);
}

void Association::AddMemberEndFunctor::operator()(ID id) const {
    if (!m_el->getMembers().count(id)) {
        m_el->getMembers().addByID(id);
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
    updateCopiedSequenceRemovedFrom(el, &Association::getMemberEnds);
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
    updateCopiedSequenceAddedTo(el, &Association::getOwnedEnds);
}

void Association::AddOwnedEndFunctor::operator()(ID id) const {
    if (!m_el->getMemberEnds().count(id)) {
        m_el->getMemberEnds().addByID(id);
    }

    if (!m_el->getFeatures().count(id)) {
        m_el->getFeatures().addByID(id);
    }

    if (!m_el->getOwnedMembers().count(id)) {
        m_el->getOwnedMembers().addByID(id);
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
    updateCopiedSequenceRemovedFrom(el, &Association::getOwnedEnds);
}

void Association::AddNavigableOwnedEndFunctor::operator()(Property& el) const {
    if (!m_el->getOwnedEnds().count(el.getID())) {
        m_el->getOwnedEnds().add(el);
    }
    updateCopiedSequenceAddedTo(el, &Association::getNavigableOwnedEnds);
}

void Association::AddNavigableOwnedEndFunctor::operator()(ID id) const {
    if (!m_el->getOwnedEnds().count(id)) {
        m_el->getOwnedEnds().addByID(id);
    }
}

void Association::RemoveNavigableOwnedEndFunctor::operator()(Property& el) const {
    if (dynamic_cast<Association*>(m_el)->getOwnedEnds().count(el.getID())) {
        dynamic_cast<Association*>(m_el)->getOwnedEnds().remove(dynamic_cast<Property&>(el));
    }
    updateCopiedSequenceRemovedFrom(el, &Association::getNavigableOwnedEnds);
}

void Association::AddEndTypeFunctor::operator()(Type& el) const {
    el.setReference(m_el);
    updateCopiedSequenceAddedTo(el, &Association::getEndType);
}

void Association::RemoveEndTypeFunctor::operator()(Type& el) const {
    el.removeReference(m_el->getID());
    updateCopiedSequenceRemovedFrom(el, &Association::getEndType);
}

void Association::setManager(UmlManager* manager) {
    Classifier::setManager(manager);
    m_relatedElements.m_manager = manager;
    m_memberEnds.m_manager = manager;
    m_ownedEnds.m_manager = manager;
    m_navigableOwnedEnds.m_manager = manager;
    m_endType.m_manager = manager;
}

void Association::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    Relationship::referencingReleased(id);
    if (m_memberEnds.count(id)) {
        m_memberEnds.elementReleased(id, &Association::getMemberEnds);
    }
    if (m_ownedEnds.count(id)) {
        m_ownedEnds.elementReleased(id, &Association::getOwnedEnds);
    }
    if (m_navigableOwnedEnds.count(id)) {
        m_navigableOwnedEnds.elementReleased(id, &Association::getNavigableOwnedEnds);
    }
    if (m_endType.count(id)) {
        m_endType.elementReleased(id, &Association::getEndType);
    }
}

void Association::referenceReindexed(ID oldID, ID newID) {
    Classifier::referenceReindexed(oldID, newID);
    Relationship::referenceReindexed(oldID, newID);
    if (m_memberEnds.count(oldID)) {
        m_memberEnds.reindex(oldID, newID, &Association::getMemberEnds);
    }
    if (m_ownedEnds.count(oldID)) {
        m_ownedEnds.reindex(oldID, newID, &Association::getOwnedEnds);
    }
    if (m_navigableOwnedEnds.count(oldID)) {
        m_navigableOwnedEnds.reindex(oldID, newID, &Association::getNavigableOwnedEnds);
    }
    if (m_endType.count(oldID)) {
        m_endType.reindex(oldID, newID, &Association::getEndType);
    }
}

void Association::restoreReferences() {
    Classifier::restoreReferences();
    Relationship::restoreReferences();
    m_memberEnds.restoreReferences();
    m_ownedEnds.restoreReferences();
    m_navigableOwnedEnds.restoreReferences();
    for (auto& end : m_memberEnds) {
        if (end.hasType()) {
            if (!m_endType.count(end.getTypeID())) {
                if (m_manager->loaded(end.getTypeID())) {
                    m_endType.add(end.getTypeRef());
                } else {
                    m_endType.addByID(end.getTypeID());
                }
            }
        }
    }
    m_endType.restoreReferences();
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
    m_memberEnds.elementErased(id);
    m_ownedEnds.elementErased(id);
    m_navigableOwnedEnds.elementErased(id);
}

Association::Association() {
    m_memberEnds.addProcedures.push_back(new AddMemberEndFunctor(this));
    m_memberEnds.removeProcedures.push_back(new RemoveMemberEndFunctor(this));
    m_ownedEnds.addProcedures.push_back(new AddOwnedEndFunctor(this));
    m_ownedEnds.removeProcedures.push_back(new RemoveOwnedEndFunctor(this));
    m_navigableOwnedEnds.addProcedures.push_back(new AddNavigableOwnedEndFunctor(this));
    m_navigableOwnedEnds.removeProcedures.push_back(new RemoveNavigableOwnedEndFunctor(this));
    m_endType.addProcedures.push_back(new AddEndTypeFunctor(this));
    m_endType.removeProcedures.push_back(new RemoveEndTypeFunctor(this));
}

Association::Association(const Association& rhs) : Classifier(rhs), PackageableElement(rhs), NamedElement(rhs), Relationship(rhs), Element(rhs) {
    m_memberEnds = rhs.m_memberEnds;
    m_memberEnds.m_el = this;
    m_memberEnds.addProcedures.clear();
    m_memberEnds.removeProcedures.clear();
    m_memberEnds.addProcedures.push_back(new AddMemberEndFunctor(this));
    m_memberEnds.removeProcedures.push_back(new RemoveMemberEndFunctor(this));
    m_ownedEnds = rhs.m_ownedEnds;
    m_ownedEnds.m_el = this;
    m_ownedEnds.addProcedures.clear();
    m_ownedEnds.removeProcedures.clear();
    m_ownedEnds.addProcedures.push_back(new AddOwnedEndFunctor(this));
    m_ownedEnds.removeProcedures.push_back(new RemoveOwnedEndFunctor(this));
    m_navigableOwnedEnds = rhs.m_navigableOwnedEnds;
    m_navigableOwnedEnds.m_el = this;
    m_navigableOwnedEnds.addProcedures.clear();
    m_navigableOwnedEnds.removeProcedures.clear();
    m_navigableOwnedEnds.addProcedures.push_back(new AddNavigableOwnedEndFunctor(this));
    m_navigableOwnedEnds.removeProcedures.push_back(new RemoveNavigableOwnedEndFunctor(this));
    m_endType = rhs.m_endType;
    m_endType.m_el = this;
    m_endType.addProcedures.clear();
    m_endType.removeProcedures.clear();
    m_endType.addProcedures.push_back(new AddEndTypeFunctor(this));
    m_endType.removeProcedures.push_back(new RemoveEndTypeFunctor(this));
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