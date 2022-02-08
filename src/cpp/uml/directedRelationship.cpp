#include "uml/directedRelationship.h"

using namespace UML;

void DirectedRelationship::init() {
    m_sources.subsets(m_relatedElements);
    m_sources.m_signature = &DirectedRelationship::getSources;
    m_sources.m_readOnly = true;
    m_targets.subsets(m_relatedElements);
    m_targets.m_signature = &DirectedRelationship::getTargets;
    m_targets.m_readOnly = true;
}

void DirectedRelationship::copy(const DirectedRelationship& rhs) {
    m_sources = rhs.m_sources;
    m_targets = rhs.m_targets;
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