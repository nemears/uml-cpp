#include "uml/connectableElement.h"

using namespace UML;

void ConnectableElement::referencingReleased(ID id) {
    TypedElement::referencingReleased(id);
    ParameterableElement::referencingReleased(id);
}

void ConnectableElement::referenceReindexed(ID oldID, ID newID) {
    TypedElement::referenceReindexed(oldID, newID);
    ParameterableElement::referenceReindexed(oldID, newID);
}

void ConnectableElement::restoreReferences() {
    TypedElement::restoreReferences();
    ParameterableElement::restoreReferences();
}

void ConnectableElement::referenceErased(ID id) {
    TypedElement::referenceErased(id);
    ParameterableElement::referenceErased(id);
}

ConnectableElement::ConnectableElement() : Element(ElementType::CONNECTABLE_ELEMENT) {}

ConnectableElement::ConnectableElement(const ConnectableElement& rhs) :
TypedElement(rhs),
ParameterableElement(rhs),
NamedElement(rhs),
Element(rhs, ElementType::CONNECTABLE_ELEMENT) {}

bool ConnectableElement::isSubClassOf(ElementType eType) const {
    bool ret = TypedElement::isSubClassOf(eType);

    if (!ret) {
        ret = ParameterableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::CONNECTABLE_ELEMENT;
    }

    return ret;
}