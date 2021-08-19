#include "uml/structuralFeature.h"

using namespace UML;

ElementType StructuralFeature::getElementType() const {
    return ElementType::STRUCTURAL_FEATURE;
}

bool StructuralFeature::isSubClassOf(ElementType eType) const {
    bool ret = TypedElement::isSubClassOf(eType);
    
    if (!ret) {
        ret = MultiplicityElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = Feature::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::STRUCTURAL_FEATURE;
    }

    return ret;
}

void StructuralFeature::restoreReleased(ID id, Element* el) {
    Feature::restoreReleased(id, el);
    TypedElement::referencingReleased(id);
}

void StructuralFeature::referencingReleased(ID id) {
    Feature::referencingReleased(id);
    TypedElement::referencingReleased(id);
}