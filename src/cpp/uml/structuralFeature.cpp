#include "uml/structuralFeature.h"

using namespace UML;

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

void StructuralFeature::restoreReferences() {
    TypedElement::restoreReferences();
    MultiplicityElement::restoreReferences();
    Feature::restoreReferences();
}

void StructuralFeature::referenceErased(ID id) {
    TypedElement::referenceErased(id);
    MultiplicityElement::referenceErased(id);
    Feature::referenceErased(id);
}

StructuralFeature::StructuralFeature() : Element(ElementType::STRUCTURAL_FEATURE) {}

StructuralFeature::StructuralFeature(const StructuralFeature& feature) : 
TypedElement(feature), 
MultiplicityElement(feature), 
Feature(feature),
NamedElement(feature),
Element(feature, ElementType::STRUCTURAL_FEATURE) {}

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