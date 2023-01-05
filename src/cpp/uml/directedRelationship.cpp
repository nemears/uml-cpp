#include "uml/directedRelationship.h"

using namespace UML;

DirectedRelationship::DirectedRelationship() : Element(ElementType::DIRECTED_RELATIONSHIP) {
    m_sources.subsets(m_relatedElements);
    m_sources.m_readOnly = true;
    m_targets.subsets(m_relatedElements);
    m_targets.m_readOnly = true;
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