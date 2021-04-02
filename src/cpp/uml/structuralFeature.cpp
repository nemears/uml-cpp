#include "uml/structuralFeature.h"

using namespace UML;

ElementType StructuralFeature::getElementType() {
    return ElementType::STRUCTURAL_FEATURE;
}

bool StructuralFeature::isSubClassOf(ElementType eType) {
    bool ret = TypedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STRUCTURAL_FEATURE;
    }

    return ret;
}