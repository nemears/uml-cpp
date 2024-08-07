#include "uml/types/relationship.h"

using namespace UML;

void Relationship::referenceErased(ID id) {
    Element::referenceErased(id);
    eraseFromSet(id, m_relatedElements);
}

Relationship::Relationship() : Element(ElementType::RELATIONSHIP) {

}

Relationship::~Relationship() {

}

ReadOnlySet<Element, Relationship>& Relationship::getRelatedElements() {
    return m_relatedElements;
}

bool Relationship::is(ElementType eType) const {
    bool ret = Element::is(eType);

    if (!ret) {
        ret = eType == ElementType::RELATIONSHIP;
    }

    return ret;
}
