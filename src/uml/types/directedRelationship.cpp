#include "uml/uml-stable.h"

using namespace UML;

DirectedRelationship::DirectedRelationship(std::size_t elementType, AbstractManager& manager) :     
    Element(elementType, manager),
    Relationship(elementType, manager)
{
    m_sources.subsets(m_relatedElements);
    m_targets.subsets(m_relatedElements);
}

ReadOnlySet<Element, DirectedRelationship>& DirectedRelationship::getTargets() {
    return m_targets;
}

ReadOnlySet<Element, DirectedRelationship>& DirectedRelationship::getSources() {
    return m_sources;
}
