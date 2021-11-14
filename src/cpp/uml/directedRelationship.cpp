#include "uml/directedRelationship.h"

using namespace UML;

void DirectedRelationship::referencingReleased(ID id) {
    Relationship::referencingReleased(id);
    m_sources.release(id);
    m_targets.release(id);
}

void DirectedRelationship::referenceReindexed(ID oldID, ID newID) {
    Relationship::referenceReindexed(oldID, newID);
    m_sources.reindex(oldID, newID);
    m_targets.reindex(oldID, newID);
}

void DirectedRelationship::restoreReferences() {
    Relationship::restoreReferences();
    // m_sources.restoreReferences();
    // m_targets.restoreReferences();
}

void DirectedRelationship::referenceErased(ID id) {
    Relationship::referenceErased(id);
    m_sources.eraseElement(id);
    m_targets.eraseElement(id);
}

void DirectedRelationship::init() {
    m_sources.subsets(m_relatedElements);
    m_sources.m_signature = &DirectedRelationship::getSources;
    m_targets.subsets(m_relatedElements);
    m_targets.m_signature = &DirectedRelationship::getTargets;
}

void DirectedRelationship::copy(const DirectedRelationship& rhs) {
    m_sources = Set<Element, DirectedRelationship>(rhs.m_sources);
    m_sources.m_el = this;
    m_targets = Set<Element, DirectedRelationship>(rhs.m_targets);
    m_targets.m_el = this;
}

DirectedRelationship::DirectedRelationship() : Element(ElementType::DIRECTED_RELATIONSHIP) {
    init();
}

DirectedRelationship::DirectedRelationship(const DirectedRelationship& relationship) : Element(ElementType::DIRECTED_RELATIONSHIP) {
    // abstract
}

DirectedRelationship::~DirectedRelationship() {
    
}

Set<Element, DirectedRelationship>& DirectedRelationship::getTargets() {
    return m_targets;
}

Set<Element, DirectedRelationship>& DirectedRelationship::getSources() {
    return m_sources;
}

bool DirectedRelationship::isSubClassOf(ElementType eType) const {
    bool ret = Relationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DIRECTED_RELATIONSHIP;
    }

    return ret;
}