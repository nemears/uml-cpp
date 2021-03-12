#include "uml/instanceSpecification.h"

using namespace UML;

InstanceSpecification::InstanceSpecification() {
    m_classifier = NULL;
    m_slots = new Sequence<Slot>;
}

InstanceSpecification::~InstanceSpecification() {
    delete m_slots;
}

Classifier* InstanceSpecification::getClassifier() {
    return m_classifier;
}

void InstanceSpecification::setClassifier(Classifier* classifier) {
    m_classifier = classifier;
}

Sequence<Slot>& InstanceSpecification::getSlots() {
    return *m_slots;
}

ElementType InstanceSpecification::getElementType() {
    return ElementType::INSTANCE_SPECIFICATION;
}