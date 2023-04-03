#include "uml/types/relationship.h"

using namespace UML;

void Relationship::referenceReindexed(ID newID) {
    Element::referenceReindexed(newID);
    m_relatedElements.reindex(newID);
}

void Relationship::referenceErased(ID id) {
    Element::referenceErased(id);
    m_relatedElements.eraseElement(id);
}

Relationship::Relationship() : Element(ElementType::RELATIONSHIP) {

}

Relationship::~Relationship() {

}

ReadOnlySet<Element, Relationship>& Relationship::getRelatedElements() {
    return m_relatedElements;
}

bool Relationship::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::RELATIONSHIP;
    }

    return ret;
}