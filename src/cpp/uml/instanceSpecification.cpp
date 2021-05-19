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

void InstanceSpecification::RemoveSlotFunctor::operator()(Element& el) const {
    if (dynamic_cast<Slot&>(el).getOwningInstance() == m_el) {
        dynamic_cast<Slot&>(el).setOwningInstance(0);
    }

    if (dynamic_cast<InstanceSpecification*>(m_el)->getOwnedElements().count(el.getID())) {
        dynamic_cast<InstanceSpecification*>(m_el)->getOwnedElements().remove(el);
    }
}

InstanceSpecification::InstanceSpecification() {
    m_classifier = NULL;
    m_slots.addProcedures.push_back(new AddSlotFunctor(this));
    m_slots.removeProcedures.push_back(new RemoveSlotFunctor(this));
}

InstanceSpecification::InstanceSpecification(const InstanceSpecification& inst) {
    m_classifier = inst.m_classifier;
    m_slots = inst.m_slots;
    m_slots.addProcedures.clear();
    m_slots.addProcedures.push_back(new AddSlotFunctor(this));
    m_slots.removeProcedures.clear();
    m_slots.removeProcedures.push_back(new RemoveSlotFunctor(this));
}

InstanceSpecification::~InstanceSpecification() {
    
}

Classifier* InstanceSpecification::getClassifier() {
    return m_classifier;
}

void InstanceSpecification::setClassifier(Classifier* classifier) {
    m_classifier = classifier;
}

Sequence<Slot>& InstanceSpecification::getSlots() {
    return m_slots;
}

ElementType InstanceSpecification::getElementType() const {
    return ElementType::INSTANCE_SPECIFICATION;
}

bool InstanceSpecification::isSubClassOf(ElementType eType) {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INSTANCE_SPECIFICATION;
    }

    return ret;
}