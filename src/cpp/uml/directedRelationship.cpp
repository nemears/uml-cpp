#include "uml/directedRelationship.h"

using namespace UML;

void DirectedRelationship::AddRelatedElementFunctor::operator()(Element& el) const {
    // add to related elements if not duplicate
    if (!dynamic_cast<DirectedRelationship*>(m_el)->getRelatedElements().count(el.getID())) {
        dynamic_cast<DirectedRelationship*>(m_el)->getRelatedElements().add(el);
    }
}

DirectedRelationship::DirectedRelationship() {
    m_targets = new Sequence<>;
    m_targets->addProcedures.push_back(new AddRelatedElementFunctor(this));
    m_sources = new Sequence<>;
    m_sources->addProcedures.push_back(new AddRelatedElementFunctor(this));
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