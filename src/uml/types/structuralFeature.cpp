#include "uml/uml-stable.h"

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

bool StructuralFeature::is(ElementType eType) const {
    bool ret = TypedElement::is(eType);
    
    if (!ret) {
        ret = MultiplicityElement::is(eType);
    }

    if (!ret) {
        ret = Feature::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::STRUCTURAL_FEATURE;
    }

    return ret;
}
