#include "uml/slot.h"

using namespace UML;

Slot::Slot() {
    m_definingFeature = 0;
    m_values = new Sequence<ValueSpecification>;
}

Slot::~Slot() {
    delete m_values;
}

Sequence<ValueSpecification>& Slot::getValues() {
    return *m_values;
}

StructuralFeature* Slot::getDefiningFeature() {
    return m_definingFeature;
}

void Slot::setDefiningFeature(StructuralFeature* definingFeature) {
    m_definingFeature = definingFeature;
}

ElementType Slot::getElementType() {
    return ElementType::SLOT;
}