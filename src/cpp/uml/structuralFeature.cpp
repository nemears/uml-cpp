#include "uml/structuralFeature.h"
#include "uml/behavior.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/structuralFeature.h"
#include "uml/deployment.h"

using namespace UML;

void StructuralFeature::referencingReleased(ID id) {
    Feature::referencingReleased(id);
    TypedElement::referencingReleased(id);
}

void StructuralFeature::referenceReindexed(ID oldID, ID newID) {
    Feature::referenceReindexed(oldID, newID);
    TypedElement::referenceReindexed(oldID, newID);
}

void StructuralFeature::reindexName(ID id, std::string newName) {
    Feature::reindexName(id, newName);
    TypedElement::reindexName(id, newName);
}

void StructuralFeature::referenceErased(ID id) {
    TypedElement::referenceErased(id);
    Feature::referenceErased(id);
}

void StructuralFeature::restoreReference(Element* el) {
    TypedElement::restoreReference(el);
    Feature::restoreReference(el);
}

StructuralFeature::StructuralFeature() : Element(ElementType::STRUCTURAL_FEATURE) {

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