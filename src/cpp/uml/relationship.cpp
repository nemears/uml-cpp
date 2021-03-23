#include "uml/relationship.h"

using namespace UML;

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