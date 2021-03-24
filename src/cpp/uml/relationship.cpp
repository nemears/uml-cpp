#include "uml/relationship.h"

using namespace UML;

void Relationship::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    for (auto const& relEl : *m_relatedElements) {
        relEl->getRelationships().reindex(oldID, newID);
    }

    Element::reindexID(oldID, newID);
}

void Relationship::AddRelationshipFunctor::operator()(Element& el) const {
    // add to related elements if not duplicate
    if (!el.getRelationships().count(m_el->getID())) {
        el.getRelationships().add(*dynamic_cast<Relationship*>(m_el));
    } else {
        throw DuplicateRelationshipException(boost::lexical_cast<string>(m_el->getID()));
    }
}

Relationship::Relationship() {
    m_relatedElements = new Sequence<>;
    m_relatedElements->addProcedures.push_back(new AddRelationshipFunctor(this));
}

Relationship::~Relationship() {
    delete m_relatedElements;
}

Sequence<>& Relationship::getRelatedElements() {
    return *m_relatedElements;
}

ElementType Relationship::getElementType() {
    return ElementType::RELATIONSHIP;
}