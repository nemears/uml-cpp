#include "uml/directedRelationship.h"

using namespace UML;

DirectedRelationship::DirectedRelationship() {
    m_targets = new Sequence<>;
    m_sources = new Sequence<>;
}

DirectedRelationship::~DirectedRelationship() {
    delete m_targets;
    delete m_sources;
}

Sequence<>& DirectedRelationship::getTargets() {
    return *m_targets;
}

Sequence<>& DirectedRelationship::getSources() {
    return *m_sources;
}

ElementType DirectedRelationship::getElementType() {
    return ElementType::DIRECTED_RELATIONSHIP;
}