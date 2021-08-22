#include "uml/relationship.h"
#include "uml/directedRelationship.h"
#include "uml/elementFunctors.h"

using namespace std;
using namespace UML;

void Relationship::reindexID(ID oldID, ID newID) {
    for (auto& relEl : m_relatedElements) {
        relEl.getRelationships().reindex(oldID, newID);
    }

    Element::reindexID(oldID, newID);
}

void Relationship::setManager(UmlManager* manager) {
    Element::setManager(manager);
    m_relatedElements.m_manager = manager;
}

void Relationship::AddRelationshipFunctor::operator()(Element& el) const {
    // add to related elements if not duplicate
    if (!el.getRelationships().count(m_el->getID())) {
        el.getRelationships().internalAdd(*dynamic_cast<Relationship*>(m_el));
    }

    m_el->as<Relationship>().getRelatedElements().updateCopiedSequenceAddedTo<Relationship>(el, &Relationship::getRelatedElements);
}

void Relationship::CheckRelatedElementsFunctor::operator()(Element& el) const {
    // add to related elements if not duplicate
    if (dynamic_cast<Relationship*>(m_el)->getRelatedElements().count(el.getID())) {
        throw DuplicateRelatedElementException(m_el->getID().string()); 
    }
}

void Relationship::RemoveRelatedElementsFunctor::operator()(Element& el) const {
    if (el.getRelationships().count(m_el->getID())) {
        el.getRelationships().internalRemove(*dynamic_cast<Relationship*>(m_el));
    }

    /** TODO: move this to directedRelationship**/
    if (m_el->isSubClassOf(ElementType::DIRECTED_RELATIONSHIP)) {
        if (dynamic_cast<DirectedRelationship*>(m_el)->getTargets().count(el.getID())) {
            dynamic_cast<DirectedRelationship*>(m_el)->getTargets().remove(el);
        }
        if (dynamic_cast<DirectedRelationship*>(m_el)->getSources().count(el.getID())) {
            dynamic_cast<DirectedRelationship*>(m_el)->getSources().remove(el);
        }
    }
    m_el->as<Relationship>().getRelatedElements().updateCopiedSequenceRemovedFrom<Relationship>(el, &Relationship::getRelatedElements);
}

Relationship::Relationship() {
    m_relatedElements.addProcedures.push_back(new AddRelationshipFunctor(this));
    m_relatedElements.addChecks.push_back(new CheckRelatedElementsFunctor(this));
    m_relatedElements.addChecks.push_back(new ReadOnlySequenceFunctor(this, "relatedElements"));
    m_relatedElements.removeProcedures.push_back(new RemoveRelatedElementsFunctor(this));
    m_relatedElements.removeChecks.push_back(new ReadOnlySequenceFunctor(this, "relatedElements"));
}

Relationship::Relationship(const Relationship& relationship) {
    m_relatedElements.m_el = this;
    m_relatedElements.addProcedures.clear();
    m_relatedElements.removeProcedures.clear();
    m_relatedElements.addChecks.clear();
    m_relatedElements.removeChecks.clear();
    m_relatedElements.addProcedures.push_back(new AddRelationshipFunctor(this));
    m_relatedElements.addChecks.push_back(new CheckRelatedElementsFunctor(this));
    m_relatedElements.addChecks.push_back(new ReadOnlySequenceFunctor(this, "relatedElements"));
    m_relatedElements.removeProcedures.push_back(new RemoveRelatedElementsFunctor(this));
    m_relatedElements.removeChecks.push_back(new ReadOnlySequenceFunctor(this, "relatedElements"));
}

Relationship::~Relationship() {

}

Sequence<>& Relationship::getRelatedElements() {
    return m_relatedElements;
}

ElementType Relationship::getElementType() const {
    return ElementType::RELATIONSHIP;
}

bool Relationship::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::RELATIONSHIP;
    }

    return ret;
}