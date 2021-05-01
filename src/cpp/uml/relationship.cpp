#include "uml/relationship.h"
#include "uml/directedRelationship.h"

using namespace std;
using namespace UML;

void Relationship::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    for (auto const& relEl : m_relatedElements) {
        relEl->getRelationships().reindex(oldID, newID);
    }

    Element::reindexID(oldID, newID);
}

void Relationship::AddRelationshipFunctor::operator()(Element& el) const {
    // add to related elements if not duplicate
    if (!el.getRelationships().count(m_el->getID())) {
        el.getRelationships().add(*dynamic_cast<Relationship*>(m_el));
    }
}

void Relationship::CheckRelatedElementsFunctor::operator()(Element& el) const {
    // add to related elements if not duplicate
    if (dynamic_cast<Relationship*>(m_el)->getRelatedElements().count(el.getID())) {
        throw DuplicateRelatedElementException(boost::lexical_cast<string>(m_el->getID())); 
    }
}

void Relationship::RemoveRelatedElementsFunctor::operator()(Element& el) const {
    if (el.getRelationships().count(m_el->getID())) {
        el.getRelationships().remove(*dynamic_cast<Relationship*>(m_el));
    }
    if (m_el->isSubClassOf(ElementType::DIRECTED_RELATIONSHIP)) {
        if (dynamic_cast<DirectedRelationship*>(m_el)->getTargets().count(el.getID())) {
            dynamic_cast<DirectedRelationship*>(m_el)->getTargets().remove(el);
        }
        if (dynamic_cast<DirectedRelationship*>(m_el)->getSources().count(el.getID())) {
            dynamic_cast<DirectedRelationship*>(m_el)->getSources().remove(el);
        }
    }
}

Relationship::Relationship() {
    m_relatedElements.addProcedures.push_back(new AddRelationshipFunctor(this));
    m_relatedElements.addChecks.push_back(new CheckRelatedElementsFunctor(this));
    m_relatedElements.removeProcedures.push_back(new RemoveRelatedElementsFunctor(this));
}

Relationship::~Relationship() {

}

Sequence<>& Relationship::getRelatedElements() {
    return m_relatedElements;
}

ElementType Relationship::getElementType() const {
    return ElementType::RELATIONSHIP;
}

bool Relationship::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::RELATIONSHIP;
    }

    return ret;
}