#include "uml/instanceSpecification.h"
#include "uml/slot.h"
#include "uml/classifier.h"
#include "uml/universalFunctions.h"
#include "uml/valueSpecification.h"

using namespace UML;

void InstanceSpecification::AddSlotFunctor::operator()(Slot& el) const {
    if (el.getOwningInstance()) {
        if (el.getOwningInstance()->getID() != m_el->getID()) {
            el.setOwningInstance(m_el);
        }
    } else {
        el.setOwningInstance(m_el);
    }

    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
}

void InstanceSpecification::RemoveSlotFunctor::operator()(Slot& el) const {
    if (el.getOwningInstance() == m_el) {
        el.setOwningInstance(0);
    }

    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
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
    m_specificationPtr = 0;
}

InstanceSpecification::InstanceSpecification(const InstanceSpecification& inst) {
    m_classifierID = inst.m_classifierID;
    if (inst.m_manager && !inst.m_classifierID.isNull()) {
        m_classifierPtr = &inst.m_manager->get<Classifier>(inst.m_classifierID);
    } else {
        m_classifierPtr = inst.m_classifierPtr;
    }
    m_specificationID = inst.m_specificationID;
    if (!m_manager) {
        m_specificationPtr = inst.m_specificationPtr;
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
        if (m_manager) {
            m_manager->removeReference(m_id, m_classifierID);
        }
        m_classifierID = ID::nullID();
        m_classifierPtr = 0;
    }

    if (classifier) {
        m_classifierID = classifier->getID();
    }
    
    if (!m_manager) {
        m_classifierPtr = classifier;
    }

    if (classifier) {
        if (m_manager) {
            m_manager->setReference(m_id, m_classifierID, this);
        }
    }
}

ValueSpecification* InstanceSpecification::getSpecification() {
    return universalGet<ValueSpecification>(m_specificationID, m_specificationPtr, m_manager);
}

void InstanceSpecification::setSpecification(ValueSpecification* specification) {
    if (!isSameOrNull(m_specificationID, specification)) {
        if (!m_specificationPtr) {
            m_specificationPtr = &m_manager->get<ValueSpecification>(m_specificationID);
        }

        if (m_ownedElements->count(m_specificationID)) {
            m_ownedElements->internalRemove(*m_specificationPtr);
        }

        m_specificationPtr = 0;
        m_specificationID = ID::nullID();
    }

    if (specification) {
        m_specificationID = specification->getID();
    }

    if (!m_manager) {
        m_specificationPtr = specification;
    }

    if (specification) {
        if (!m_ownedElements->count(specification->getID())) {
            m_ownedElements->internalAdd(*specification);
        }
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

void InstanceSpecification::restoreReleased(ID id, Element* released) {
    Element::restoreReleased(id, released);
}

void InstanceSpecification::referencingReleased(ID id) {
    Element::referencingReleased(id);
    if (m_classifierID == id) {
        m_classifierPtr = 0;
    }
}