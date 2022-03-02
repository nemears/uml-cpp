#include "uml/structuralFeature.h"
#include "uml/class.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"

using namespace UML;

void StructuralFeature::referencingReleased(ID id) {
    Feature::referencingReleased(id);
    TypedElement::referencingReleased(id);
}

void StructuralFeature::referenceReindexed(ID oldID, ID newID) {
    Feature::referenceReindexed(oldID, newID);
    TypedElement::referenceReindexed(oldID, newID);
}

void StructuralFeature::reindexName(std::string oldName, std::string newName) {
    Feature::reindexName(oldName, newName);
    TypedElement::reindexName(oldName, newName);
}

void StructuralFeature::referenceErased(ID id) {
    TypedElement::referenceErased(id);
    Feature::referenceErased(id);
}

StructuralFeature::StructuralFeature() : Element(ElementType::STRUCTURAL_FEATURE) {

}

bool StructuralFeature::isSubClassOf(ElementType eType) const {
    bool ret = TypedElement::isSubClassOf(eType);
    
    // if (!ret) {
    //     ret = MultiplicityElement::isSubClassOf(eType);
    // }

    if (!ret) {
        ret = Feature::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::STRUCTURAL_FEATURE;
    }

    return ret;
}