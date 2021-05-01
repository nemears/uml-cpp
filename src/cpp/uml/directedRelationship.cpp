#include "uml/directedRelationship.h"

using namespace UML;

void DirectedRelationship::AddRelatedElementFunctor::operator()(Element& el) const {
    // add to related elements if not duplicate
    if (!dynamic_cast<DirectedRelationship*>(m_el)->getRelatedElements().count(el.getID())) {
        dynamic_cast<DirectedRelationship*>(m_el)->getRelatedElements().add(el);
    }
}

void DirectedRelationship::RemoveRelatedElementFunctor::operator()(Element& el) const {
    if (dynamic_cast<DirectedRelationship*>(m_el)->getRelatedElements().count(el.getID())) {
        dynamic_cast<DirectedRelationship*>(m_el)->getRelatedElements().remove(el);
    }
}

DirectedRelationship::DirectedRelationship() {
    m_targets.addProcedures.push_back(new AddRelatedElementFunctor(this));
    m_targets.removeProcedures.push_back(new RemoveRelatedElementFunctor(this));
    m_sources.addProcedures.push_back(new AddRelatedElementFunctor(this));
    m_sources.removeProcedures.push_back(new RemoveRelatedElementFunctor(this));
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

bool DirectedRelationship::isSubClassOf(ElementType eType) {
    bool ret = Relationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DIRECTED_RELATIONSHIP;
    }

    return ret;
}