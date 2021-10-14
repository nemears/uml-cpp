#include "uml/directedRelationship.h"

using namespace UML;

void DirectedRelationship::AddRelatedElementFunctor::operator()(Element& el) const {
    // add to related elements if not duplicate
    if (!dynamic_cast<DirectedRelationship*>(m_el)->getRelatedElements().count(el.getID())) {
        dynamic_cast<DirectedRelationship*>(m_el)->getRelatedElements().internalAdd(el);
    }

    if (!el.getDirectedRelationships().count(m_el->getID())) {
        el.getDirectedRelationships().add(*dynamic_cast<DirectedRelationship*>(m_el));
    }
}

void DirectedRelationship::AddRelatedElementFunctor::operator()(ID id) const {
    if (!m_el->getRelatedElements().count(id)) {
        m_el->getRelatedElements().addByID(id);
    }
}

void DirectedRelationship::RemoveRelatedElementFunctor::operator()(Element& el) const {
    if (dynamic_cast<DirectedRelationship*>(m_el)->getRelatedElements().count(el.getID())) {
        dynamic_cast<DirectedRelationship*>(m_el)->getRelatedElements().internalRemove(el);
    }
    
    if (el.getDirectedRelationships().count(m_el->getID())) {
        el.getDirectedRelationships().remove(*dynamic_cast<DirectedRelationship*>(m_el));
    }
}

void DirectedRelationship::AddSourcesFunctor::operator()(Element& el) const {
    updateCopiedSequenceAddedTo(el, &DirectedRelationship::getSources);
}

void DirectedRelationship::RemoveSourcesFunctor::operator()(Element& el) const {
    updateCopiedSequenceRemovedFrom(el, &DirectedRelationship::getSources);
}

void DirectedRelationship::AddTargetsFunctor::operator()(Element& el) const {
    updateCopiedSequenceAddedTo(el, &DirectedRelationship::getTargets);
}

void DirectedRelationship::RemoveTargetsFunctor::operator()(Element& el) const {
    updateCopiedSequenceRemovedFrom(el, &DirectedRelationship::getTargets);
}

void DirectedRelationship::setManager(UmlManager* manager) {
    Relationship::setManager(manager);
    m_sources.m_manager = manager;
    m_targets.m_manager = manager;
}

void DirectedRelationship::referencingReleased(ID id) {
    Relationship::referencingReleased(id);
    m_sources.elementReleased(id, &DirectedRelationship::getSources);
    m_targets.elementReleased(id, &DirectedRelationship::getTargets);
}

void DirectedRelationship::referenceReindexed(ID oldID, ID newID) {
    Relationship::referenceReindexed(oldID, newID);
    if (m_sources.count(oldID)) {
        m_sources.reindex(oldID, newID, &DirectedRelationship::getSources);
    }
    if (m_targets.count(oldID)) {
        m_targets.reindex(oldID, newID, &DirectedRelationship::getTargets);
    }
}

void DirectedRelationship::restoreReferences() {
    Relationship::restoreReferences();
    m_sources.restoreReferences();
    m_targets.restoreReferences();
}

void DirectedRelationship::referenceErased(ID id) {
    Relationship::referenceErased(id);
    m_sources.elementErased(id);
    m_targets.elementErased(id);
}

DirectedRelationship::DirectedRelationship() {
    m_targets.addProcedures.push_back(new AddRelatedElementFunctor(this));
    m_targets.removeProcedures.push_back(new RemoveRelatedElementFunctor(this));
    m_sources.addProcedures.push_back(new AddRelatedElementFunctor(this));
    m_sources.removeProcedures.push_back(new RemoveRelatedElementFunctor(this));
    m_targets.addProcedures.push_back(new AddTargetsFunctor(this));
    m_targets.removeProcedures.push_back(new RemoveTargetsFunctor(this));
    m_sources.addProcedures.push_back(new AddSourcesFunctor(this));
    m_sources.removeProcedures.push_back(new RemoveSourcesFunctor(this));
}

DirectedRelationship::DirectedRelationship(const DirectedRelationship& relationship) {
    m_targets = relationship.m_targets;
    m_sources = relationship.m_sources;
    m_targets.m_el = this;
    m_sources.m_el = this;
    m_targets.addProcedures.clear();
    m_targets.removeProcedures.clear();
    m_sources.addProcedures.clear();
    m_sources.removeProcedures.clear();
    m_targets.addProcedures.push_back(new AddRelatedElementFunctor(this));
    m_targets.removeProcedures.push_back(new RemoveRelatedElementFunctor(this));
    m_sources.addProcedures.push_back(new AddRelatedElementFunctor(this));
    m_sources.removeProcedures.push_back(new RemoveRelatedElementFunctor(this));
    m_targets.addProcedures.push_back(new AddTargetsFunctor(this));
    m_targets.removeProcedures.push_back(new RemoveTargetsFunctor(this));
    m_sources.addProcedures.push_back(new AddSourcesFunctor(this));
    m_sources.removeProcedures.push_back(new RemoveSourcesFunctor(this));
}

DirectedRelationship::~DirectedRelationship() {
    
}

Sequence<>& DirectedRelationship::getTargets() {
    return m_targets;
}

Sequence<>& DirectedRelationship::getSources() {
    return m_sources;
}

ElementType DirectedRelationship::getElementType() const {
    return ElementType::DIRECTED_RELATIONSHIP;
}

bool DirectedRelationship::isSubClassOf(ElementType eType) const {
    bool ret = Relationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DIRECTED_RELATIONSHIP;
    }

    return ret;
}