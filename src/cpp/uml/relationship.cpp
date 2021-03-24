#include "uml/relationship.h"

using namespace UML;

void Relationship::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    for (auto const& relEl : *m_relatedElements) {
        relEl->getRelationships().reindex(oldID, newID);
    }

    Element::reindexID(oldID, newID);
}

Relationship::Relationship() {
    m_relatedElements = new Sequence<>;
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