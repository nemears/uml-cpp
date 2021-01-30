#include "uml/slot.h"

using namespace UML;

StructuralFeature* Slot::getDefiningFeature() {
    return definingFeature;
}

void Slot::setDefiningFeature(StructuralFeature* definingFeature) {
    this->definingFeature = definingFeature;
}

ElementType Slot::getElementType() {
    return ElementType::SLOT;
}