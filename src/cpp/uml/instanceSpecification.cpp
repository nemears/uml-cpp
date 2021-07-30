#include "uml/instanceSpecification.h"
#include "uml/slot.h"
#include "uml/classifier.h"
#include "uml/universalFunctions.h"

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
        m_el->getOwnedElements().internalAdd(el);
    }
}

void InstanceSpecification::RemoveSlotFunctor::operator()(Element& el) const {
    if (dynamic_cast<Slot&>(el).getOwningInstance() == m_el) {
        dynamic_cast<Slot&>(el).setOwningInstance(0);
    }

    if (dynamic_cast<InstanceSpecification*>(m_el)->getOwnedElements().count(el.getID())) {
        dynamic_cast<InstanceSpecification*>(m_el)->getOwnedElements().internalRemove(el);
    }
}

void InstanceSpecification::setManager(UmlManager* manager) {
    PackageableElement::setManager(manager);
    m_slots.m_manager = manager;
}

InstanceSpecification::InstanceSpecification() {
    m_classifierPtr = 0;
    m_slots.addProcedures.push_back(new AddSlotFunctor(this));
    m_slots.removeProcedures.push_back(new RemoveSlotFunctor(this));
}

InstanceSpecification::InstanceSpecification(const InstanceSpecification& inst) {
    m_classifierID = inst.m_classifierID;
    if (inst.m_manager && !inst.m_classifierID.isNull()) {
        m_classifierPtr = &inst.m_manager->get<Classifier>(inst.m_classifierID);
    } else {
        m_classifierPtr = inst.m_classifierPtr;
    }
    m_slots = inst.m_slots;
    m_slots.addProcedures.clear();
    m_slots.addProcedures.push_back(new AddSlotFunctor(this));
    m_slots.removeProcedures.clear();
    m_slots.removeProcedures.push_back(new RemoveSlotFunctor(this));
}

InstanceSpecification::~InstanceSpecification() {
    
}

Classifier* InstanceSpecification::getClassifier() {
    return universalGet<Classifier>(m_classifierID, m_classifierPtr, m_manager);
}

void InstanceSpecification::setClassifier(Classifier* classifier) {
    if (!m_classifierID.isNull()) {
        // if (!m_classifierPtr) {
        //     m_classifierPtr = &m_manager->get<Classifier>(m_classifierID);
        // }
        m_classifierID = ID::nullID();
        m_classifierPtr = 0;
    }

    if (classifier) {
        m_classifierID = classifier->getID();
    }
    
    if (!m_manager) {
        m_classifierPtr = classifier;
    }
}

Sequence<Slot>& InstanceSpecification::getSlots() {
    return m_slots;
}

ElementType InstanceSpecification::getElementType() const {
    return ElementType::INSTANCE_SPECIFICATION;
}

bool InstanceSpecification::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = DeploymentTarget::isSubClassOf(eType);
    }

    if (!ret) {
        ret = DeployedArtifact::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::INSTANCE_SPECIFICATION;
    }

    return ret;
}