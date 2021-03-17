#include "uml/slot.h"
#include "uml/instanceSpecification.h"

using namespace UML;

Slot::Slot() {
    m_definingFeature = 0;
    m_values = new Sequence<ValueSpecification>;
    m_owningInstance = NULL;
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

InstanceSpecification* Slot::getOwningInstance() {
    return m_owningInstance;
}

void Slot::setOwningInstance(InstanceSpecification* inst) {
    m_owningInstance = inst;
}

ElementType Slot::getElementType() {
    return ElementType::SLOT;
}