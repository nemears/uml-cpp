#include "uml/structuralFeature.h"

using namespace UML;

void StructuralFeature::restoreReleased(ID id, Element* el) {
    Feature::restoreReleased(id, el);
    TypedElement::restoreReleased(id, el);
    MultiplicityElement::restoreReleased(id, el);
}

void StructuralFeature::referencingReleased(ID id) {
    Feature::referencingReleased(id);
    TypedElement::referencingReleased(id);
    MultiplicityElement::referencingReleased(id);
}

void StructuralFeature::referenceReindexed(ID oldID, ID newID) {
    Feature::referenceReindexed(oldID, newID);
    TypedElement::referenceReindexed(oldID, newID);
    MultiplicityElement::referenceReindexed(oldID, newID);
}

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