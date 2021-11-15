#include "uml/relationship.h"

using namespace UML;

void Relationship::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_relatedElements.release(id);
}

void Relationship::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_relatedElements.reindex(oldID, newID);
}

void Relationship::restoreReferences() {
    Element::restoreReferences();
    // m_relatedElements.restoreReferences();
}

void Relationship::referenceErased(ID id) {
    Element::referenceErased(id);
    m_relatedElements.eraseElement(id);
}

void Relationship::init() {
    m_relatedElements.m_readOnly = true;
    m_relatedElements.m_signature = &Relationship::getRelatedElements;
}

void Relationship::copy(const Relationship& rhs) {
    m_relatedElements = Set<Element, Relationship>(rhs.m_relatedElements);
    m_relatedElements.m_el = this;
}

Relationship::Relationship() : Element(ElementType::RELATIONSHIP) {
    init();
}

Relationship::Relationship(const Relationship& relationship) : Element(ElementType::RELATIONSHIP) {
    // abstract
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