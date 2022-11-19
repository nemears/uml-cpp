#include "uml/relationship.h"

using namespace UML;

void Relationship::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_relatedElements.reindex(newID);
}

void Relationship::referenceErased(ID id) {
    Element::referenceErased(id);
    m_relatedElements.eraseElement(id);
}

void Relationship::init() {
    m_relatedElements.m_readOnly = true;
}

Relationship::Relationship() : Element(ElementType::RELATIONSHIP) {
    init();
}

Relationship::~Relationship() {

}

Set<Element, Relationship>& Relationship::getRelatedElements() {
    return m_relatedElements;
}

bool Relationship::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::RELATIONSHIP;
    }

    return ret;
}