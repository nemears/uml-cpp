#include "uml/types/structuralFeature.h"
#include "uml/types/behavior.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/structuralFeature.h"
#include "uml/types/deployment.h"

using namespace UML;

void StructuralFeature::referenceErased(ID id) {
    TypedElement::referenceErased(id);
    Feature::referenceErased(id);
}

StructuralFeature::StructuralFeature() : Element(ElementType::STRUCTURAL_FEATURE) {

}

bool StructuralFeature::isReadOnly() const {
    return m_readOnly;
}

void StructuralFeature::setReadOnly(bool readOnly) {
    m_readOnly = readOnly;
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