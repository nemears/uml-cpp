#include "uml/instanceSpecification.h"

using namespace UML;

void InstanceSpecification::AddSlotFunctor::operator()(Element& el) const {
    if (dynamic_cast<Slot&>(el).getOwningInstance()) {
        if (dynamic_cast<Slot&>(el).getOwningInstance()->getID() != m_el->getID()) {
            dynamic_cast<Slot&>(el).setOwningInstance(dynamic_cast<InstanceSpecification*>(m_el));
        }
    } else {
        dynamic_cast<Slot&>(el).setOwningInstance(dynamic_cast<InstanceSpecification*>(m_el));
    }

    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().add(el);
    }
}

InstanceSpecification::InstanceSpecification() {
    m_classifier = NULL;
    m_slots = new Sequence<Slot>;
    m_slots->addProcedures.push_back(new AddSlotFunctor(this));
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