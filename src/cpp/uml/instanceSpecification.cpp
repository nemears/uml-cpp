#include "uml/instanceSpecification.h"
#include "uml/slot.h"
#include "uml/classifier.h"
#include "uml/valueSpecification.h"

using namespace UML;

void InstanceSpecification::RemoveClassifierProcedure::operator()(Classifier* el) const {
    if (el->m_manager) {
        el->removeReference(m_me->getID());
    }
}

void InstanceSpecification::AddClassifierProcedure::operator()(Classifier* el) const {
    if (el->m_manager) {
        el->setReference(m_me);
    }
}

void InstanceSpecification::RemoveSpecificationProcedure::operator()(ValueSpecification* el) const {
    if (m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalRemove(*el);
    }
}

void InstanceSpecification::AddSpecificationProcedure::operator()(ValueSpecification* el) const {
    if (!m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalAdd(*el);
    }
}

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

void InstanceSpecification::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    if (m_classifier.id() == oldID) {
        m_classifier.reindex(oldID, newID);
    }
    if (m_slots.count(oldID)) {
        m_slots.reindex(oldID, newID);
    }
    if (m_specification.id() == oldID) {
        m_specification.reindex(oldID, newID);
    }
}

InstanceSpecification::InstanceSpecification() {
    m_classifier.m_signature = &InstanceSpecification::m_classifier;
    m_classifier.m_removeProcedures.push_back(new RemoveClassifierProcedure(this));
    m_classifier.m_addProcedures.push_back(new AddClassifierProcedure(this));
    m_slots.addProcedures.push_back(new AddSlotFunctor(this));
    m_slots.removeProcedures.push_back(new RemoveSlotFunctor(this));
    m_specification.m_signature = &InstanceSpecification::m_specification;
    m_specification.m_removeProcedures.push_back(new RemoveSpecificationProcedure(this));
    m_specification.m_addProcedures.push_back(new AddSpecificationProcedure(this));
}

InstanceSpecification::InstanceSpecification(const InstanceSpecification& inst) : PackageableElement(inst), DeploymentTarget(inst), DeployedArtifact(inst), ParameterableElement(inst), NamedElement(inst), Element(inst) {
    m_classifier = inst.m_classifier;
    m_classifier.m_me = this;
    m_classifier.m_removeProcedures.clear();
    m_classifier.m_addProcedures.clear();
    m_classifier.m_removeProcedures.push_back(new RemoveClassifierProcedure(this));
    m_classifier.m_addProcedures.push_back(new AddClassifierProcedure(this));
    m_specification = inst.m_specification;
    m_specification.m_me = this;
    m_specification.m_removeProcedures.clear();
    m_specification.m_addProcedures.clear();
    m_specification.m_removeProcedures.push_back(new RemoveSpecificationProcedure(this));
    m_specification.m_addProcedures.push_back(new AddSpecificationProcedure(this));
    m_slots = inst.m_slots;
    m_slots.m_el = this;
    m_slots.addProcedures.clear();
    m_slots.addProcedures.push_back(new AddSlotFunctor(this));
    m_slots.removeProcedures.clear();
    m_slots.removeProcedures.push_back(new RemoveSlotFunctor(this));
}

InstanceSpecification::~InstanceSpecification() {
    
}

Classifier* InstanceSpecification::getClassifier() {
    return m_classifier.get();
}
Classifier& InstanceSpecification::getClassifierRef() {
    return m_classifier.getRef();
}

bool InstanceSpecification::hasClassifier() const {
    return m_classifier.has();
}

void InstanceSpecification::setClassifier(Classifier* classifier) {
    m_classifier.set(classifier);
}

void InstanceSpecification::setClassifier(Classifier& classifier) {
    m_classifier.set(classifier);
}

ValueSpecification* InstanceSpecification::getSpecification() {
    return m_specification.get();
}

ValueSpecification& InstanceSpecification::getSpecificationRef() {
    return m_specification.getRef();
}

bool InstanceSpecification::hasSpecification() const {
    return m_specification.has();
}

void InstanceSpecification::setSpecification(ValueSpecification* specification) {
    m_specification.set(specification);
}

void InstanceSpecification::setSpecification(ValueSpecification& specification) {
    m_specification.set(specification);
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
    PackageableElement::restoreReleased(id, released);
    if (m_classifier.id() == id) {
        
    }
}

void InstanceSpecification::referencingReleased(ID id) {
    PackageableElement::referencingReleased(id);
    if (m_classifier.id() == id) {
        m_classifier.release();
    }
    if (m_specification.id() == id) {
        m_specification.release();
    }
}